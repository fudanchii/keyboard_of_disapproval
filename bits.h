#ifndef _H_BITS
#define _H_BITS


#define bitclr1(sfr, bit)                    sfr &= ~_BV(bit)
#define bitclr2(sfr, a, b)                   sfr &= (~_BV(a)) & (~_BV(b))
#define bitclr3(sfr, a, b, c)                sfr &= (~_BV(a)) & (~_BV(b)) & (~_BV(c))
#define bitclr4(sfr, a, b, c, d)             sfr &= (~_BV(a)) & (~_BV(b)) & (~_BV(c)) & (~_BV(d))
#define bitclr5(sfr, a, b, c, d, e)          sfr &= (~_BV(a)) & (~_BV(b)) & (~_BV(c)) & (~_BV(d)) & (~_BV(e))
#define bitclr6(sfr, a, b, c, d, e, f)       sfr &= (~_BV(a)) & (~_BV(b)) & (~_BV(c)) & (~_BV(d)) & (~_BV(e)) & (~_BV(f))
#define bitclr7(sfr, a, b, c, d, e, f, g)    sfr &= (~_BV(a)) & (~_BV(b)) & (~_BV(c)) & (~_BV(d)) & (~_BV(e)) & (~_BV(f)) & (~_BV(g))
#define bitclr8(sfr, a, b, c, d, e, f, g, h) sfr &= (~_BV(a)) & (~_BV(b)) & (~_BV(c)) & (~_BV(d)) & (~_BV(e)) & (~_BV(f)) & (~_BV(g)) & (~_BV(h))

#define bitset1(sfr, bit)                    sfr |= _BV(bit)
#define bitset2(sfr, a, b)                   sfr |= _BV(a) | _BV(b)
#define bitset3(sfr, a, b, c)                sfr |= _BV(a) | _BV(b) | _BV(c)
#define bitset4(sfr, a, b, c, d)             sfr |= _BV(a) | _BV(b) | _BV(c) | _BV(d)
#define bitset5(sfr, a, b, c, d, e)          sfr |= _BV(a) | _BV(b) | _BV(c) | _BV(d) | _BV(e)
#define bitset6(sfr, a, b, c, d, e, f)       sfr |= _BV(a) | _BV(b) | _BV(c) | _BV(d) | _BV(e) | _BV(f)
#define bitset7(sfr, a, b, c, d, e, f, g)    sfr |= _BV(a) | _BV(b) | _BV(c) | _BV(d) | _BV(e) | _BV(f) | _BV(g)
#define bitset8(sfr, a, b, c, d, e, f, g, h) sfr |= _BV(a) | _BV(b) | _BV(c) | _BV(d) | _BV(e) | _BV(f) | _BV(g) | _BV(h)

#endif
