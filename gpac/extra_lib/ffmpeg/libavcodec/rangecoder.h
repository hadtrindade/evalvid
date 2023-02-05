/*
 * Range coder
 * Copyright (c) 2004 Michael Niedermayer <michaelni@gmx.at>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

/**
 * @file rangecoder.h
 * Range coder.
 */

typedef struct RangeCoder{
    int low;
    int range;
    int outstanding_count;
    int outstanding_byte;
    uint8_t zero_state[256];
    uint8_t  one_state[256];
    uint8_t *bytestream_start;
    uint8_t *bytestream;
    uint8_t *bytestream_end;
}RangeCoder;

void ff_init_range_encoder(RangeCoder *c, uint8_t *buf, int buf_size);
void ff_init_range_decoder(RangeCoder *c, const uint8_t *buf, int buf_size);
int ff_rac_terminate(RangeCoder *c);
void ff_build_rac_states(RangeCoder *c, int factor, int max_p);

static inline void renorm_encoder(RangeCoder *c){
    //FIXME optimize
    while(c->range < 0x100){
        if(c->outstanding_byte < 0){
            c->outstanding_byte= c->low>>8;
        }else if(c->low <= 0xFF00){
            *c->bytestream++ = c->outstanding_byte;
            for(;c->outstanding_count; c->outstanding_count--)
                *c->bytestream++ = 0xFF;
            c->outstanding_byte= c->low>>8;
        }else if(c->low >= 0x10000){
            *c->bytestream++ = c->outstanding_byte + 1;
            for(;c->outstanding_count; c->outstanding_count--)
                *c->bytestream++ = 0x00;
            c->outstanding_byte= (c->low>>8) & 0xFF;
        }else{
            c->outstanding_count++;
        }

        c->low = (c->low & 0xFF)<<8;
        c->range <<= 8;
    }
}

static inline void put_rac(RangeCoder *c, uint8_t * const state, int bit){
    int range1= (c->range * (*state)) >> 8;

    assert(*state);
    assert(range1 < c->range);
    assert(range1 > 0);
    if(!bit){
        c->range -= range1;
        *state= c->zero_state[*state];
    }else{
        c->low += c->range - range1;
        c->range = range1;
        *state= c->one_state[*state];
    }

    renorm_encoder(c);
}

static inline void refill(RangeCoder *c){
    if(c->range < 0x100){
        c->range <<= 8;
        c->low <<= 8;
        if(c->bytestream < c->bytestream_end)
            c->low+= c->bytestream[0];
        c->bytestream++;
    }
}

static inline int get_rac(RangeCoder *c, uint8_t * const state){
    int range1= (c->range * (*state)) >> 8;
    int attribute_unused one_mask;

    c->range -= range1;
#if 1
    if(c->low < c->range){
        *state= c->zero_state[*state];
        refill(c);
        return 0;
    }else{
        c->low -= c->range;
        *state= c->one_state[*state];
        c->range = range1;
        refill(c);
        return 1;
    }
#else
    one_mask= (c->range - c->low-1)>>31;

    c->low -= c->range & one_mask;
    c->range += (range1 - c->range) & one_mask;

    *state= c->zero_state[(*state) + (256&one_mask)];

    refill(c);

    return one_mask&1;
#endif
}

