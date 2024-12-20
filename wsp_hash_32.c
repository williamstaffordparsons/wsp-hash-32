#include "wsp_hash_32.h"

uint32_t _wsp_hash_read_32(const uint8_t *input, unsigned long i) {
  uint32_t input_aligned;

  memcpy(&input_aligned, &input[i], sizeof(input_aligned));
  return input_aligned;
}

uint32_t wsp_hash_32(unsigned long input_count, const uint8_t *input) {
  uint32_t a = 1;
  uint32_t b = 11;
  uint32_t c = 111;
  uint32_t d = 1111;
  uint32_t e = 11111;
  uint32_t f = 111111;
  uint32_t g = 1111111;
  uint32_t h = 11111111;
  uint32_t mix = 1111111111;
  uint32_t mix_offset = 111111111;
  unsigned long input_count_capture = input_count;
  unsigned long i = 0;

  if (input_count >= 32) {
    i = 31;

    while (i < input_count) {
      mix += a + b + c + d + e + f + g + h;
      a += _wsp_hash_read_32(input, i - 3) + ((a << 8) | (a >> 24)) + mix;
      b += _wsp_hash_read_32(input, i - 7) + ((b << 23) | (b >> 9));
      c += _wsp_hash_read_32(input, i - 11) + ((c << 10) | (c >> 22));
      d += _wsp_hash_read_32(input, i - 15) + ((d << 21) | (d >> 11));
      e += _wsp_hash_read_32(input, i - 19) + ((e << 12) | (e >> 20));
      f += _wsp_hash_read_32(input, i - 23) + ((f << 19) | (f >> 13));
      g += _wsp_hash_read_32(input, i - 27) + ((g << 14) | (g >> 18));
      h += _wsp_hash_read_32(input, i - 31) + ((h << 17) | (h >> 15));
      i += 32;
    }

    if (i >= input_count) {
      i -= 32;
    }

    mix_offset += a + b + c + d + e + f + g + h;
    i++;
  }

  if ((input_count - i) >= 16) {
    i += 16;
    a += _wsp_hash_read_32(input, i - 16) + ((a << 8) | (a >> 24));
    b += _wsp_hash_read_32(input, i - 12) + ((b << 23) | (b >> 9));
    c += _wsp_hash_read_32(input, i - 8) + ((c << 10) | (c >> 22));
    d += _wsp_hash_read_32(input, i - 4) + ((d << 21) | (d >> 11));
    mix += a + b + c + d;
  }

  if ((input_count - i) >= 8) {
    i += 8;
    a += _wsp_hash_read_32(input, i - 8) + ((a << 8) | (a >> 24));
    b += _wsp_hash_read_32(input, i - 4) + ((b << 23) | (b >> 9));
    mix += a + b;
  }

  if (i != input_count) {
    mix += (a << 8) | (a >> 24);
    input_count -= i;

    if (input_count >= 4) {
      a += _wsp_hash_read_32(input, i) + ((a << 23) | (a >> 9));

      if (input_count != 4) {
        mix_offset += a + b;

        switch (input_count) {
          case 7:
            b += input[i + 6] << 16;
          case 6:
            b += input[i + 5] << 8;
          case 5:
            b += input[i + 4];
        }
      }
    }

    switch (input_count) {
      case 3:
        a ^= input[i + 2] << 16;
      case 2:
        a ^= input[i + 1] << 8;
      case 1:
        a ^= input[i];
    }
  }

  a += b + mix_offset;
  mix += (a << 8) | (a >> 24);

  if (input_count_capture > 12) {
    mix += ((c << 22) | (c >> 10)) + ((d << 11) | (d >> 21));

    if (input_count_capture >= 32) {
      mix_offset += ((e << 20) | (e >> 12)) ^ ((g << 18) | (g >> 14));
      mix += (h << 15) | (h >> 17);
    }
  }

  mix_offset += mix + input_count_capture;
  mix += ((a << 13) | (a >> 19)) ^ h;
  b ^= mix_offset;
  mix_offset += a ^ ((b << 18) | (b >> 14));

  if (input_count_capture >= 8) {
    c ^= mix;
    mix += b ^ ((c << 15) | (c >> 17));
    e ^= mix_offset;
    mix_offset += d ^ ((e << 18) | (e >> 14));

    if (input_count_capture >= 32) {
      f ^= ((e << 18) | (e >> 14)) + mix;
      mix_offset += e ^ ((f << 19) | (f >> 13));
      g ^= mix_offset;
      mix += f ^ ((g << 21) | (g >> 11));
    }
  }

  return mix + mix_offset;
}

void wsp_hash_32_initialize(struct wsp_hash_32_s *s) {
  s->a = 1;
  s->b = 11;
  s->c = 111;
  s->d = 1111;
  s->e = 11111;
  s->f = 111111;
  s->g = 1111111;
  s->h = 11111111;
  s->mix = 1111111111;
  s->mix_offset = 111111111;
  s->input_count_capture = 0;
}

void wsp_hash_32_transform(unsigned long i, unsigned long input_count,
                           const uint8_t *input, struct wsp_hash_32_s *s) {
  s->input_count_capture += input_count;

  if (input_count >= 32) {
    i = 31;

    while (i < input_count) {
      s->mix += s->a + s->b + s->c + s->d + s->e + s->f + s->g + s->h;
      s->a += _wsp_hash_read_32(input, i - 3) + ((s->a << 8) | (s->a >> 24))
        + s->mix;
      s->b += _wsp_hash_read_32(input, i - 7) + ((s->b << 23) | (s->b >> 9));
      s->c += _wsp_hash_read_32(input, i - 11) + ((s->c << 10) | (s->c >> 22));
      s->d += _wsp_hash_read_32(input, i - 15) + ((s->d << 21) | (s->d >> 11));
      s->e += _wsp_hash_read_32(input, i - 19) + ((s->e << 12) | (s->e >> 20));
      s->f += _wsp_hash_read_32(input, i - 23) + ((s->f << 19) | (s->f >> 13));
      s->g += _wsp_hash_read_32(input, i - 27) + ((s->g << 14) | (s->g >> 18));
      s->h += _wsp_hash_read_32(input, i - 31) + ((s->h << 17) | (s->h >> 15));
      i += 32;
    }

    if (i >= input_count) {
      i -= 32;
    }

    s->mix_offset += s->a + s->b + s->c + s->d + s->e + s->f + s->g + s->h;
    i++;
  }

  if ((input_count - i) >= 16) {
    i += 16;
    s->a += _wsp_hash_read_32(input, i - 16) + ((s->a << 8) | (s->a >> 24));
    s->b += _wsp_hash_read_32(input, i - 12) + ((s->b << 23) | (s->b >> 9));
    s->c += _wsp_hash_read_32(input, i - 8) + ((s->c << 10) | (s->c >> 22));
    s->d += _wsp_hash_read_32(input, i - 4) + ((s->d << 21) | (s->d >> 11));
    s->mix += s->a + s->b + s->c + s->d;
  }

  if ((input_count - i) >= 8) {
    i += 8;
    s->a += _wsp_hash_read_32(input, i - 8) + ((s->a << 8) | (s->a >> 24));
    s->b += _wsp_hash_read_32(input, i - 4) + ((s->b << 23) | (s->b >> 9));
    s->mix += s->a + s->b;
  }

  if (i != input_count) {
    s->mix += (s->a << 8) | (s->a >> 24);
    input_count -= i;

    if (input_count >= 4) {
      s->a += _wsp_hash_read_32(input, i) + ((s->a << 23) | (s->a >> 9));

      if (input_count != 4) {
        s->mix_offset += s->a + s->b;

        switch (input_count) {
          case 7:
            s->b += input[i + 6] << 16;
          case 6:
            s->b += input[i + 5] << 8;
          case 5:
            s->b += input[i + 4];
        }
      }
    }

    switch (input_count) {
      case 3:
        s->a ^= input[i + 2] << 16;
      case 2:
        s->a ^= input[i + 1] << 8;
      case 1:
        s->a ^= input[i];
    }
  }
}

void wsp_hash_32_finalize(struct wsp_hash_32_s *s) {
  s->a += s->b + s->mix_offset;
  s->mix += (s->a << 8) | (s->a >> 24);

  if (s->input_count_capture > 12) {
    s->mix += ((s->c << 22) | (s->c >> 10)) + ((s->d << 11) | (s->d >> 21));

    if (s->input_count_capture >= 32) {
      s->mix_offset += ((s->e << 20) | (s->e >> 12))
        ^ ((s->g << 18) | (s->g >> 14));
      s->mix += (s->h << 15) | (s->h >> 17);
    }
  }

  s->mix_offset += s->mix + s->input_count_capture;
  s->mix += ((s->a << 13) | (s->a >> 19)) ^ s->h;
  s->b ^= s->mix_offset;
  s->mix_offset += s->a ^ ((s->b << 18) | (s->b >> 14));

  if (s->input_count_capture >= 8) {
    s->c ^= s->mix;
    s->mix += s->b ^ ((s->c << 15) | (s->c >> 17));
    s->e ^= s->mix_offset;
    s->mix_offset += s->d ^ ((s->e << 18) | (s->e >> 14));

    if (s->input_count_capture >= 32) {
      s->f ^= ((s->e << 18) | (s->e >> 14)) + s->mix;
      s->mix_offset += s->e ^ ((s->f << 19) | (s->f >> 13));
      s->g ^= s->mix_offset;
      s->mix += s->f ^ ((s->g << 21) | (s->g >> 11));
    }
  }

  s->mix += s->mix_offset;
}
