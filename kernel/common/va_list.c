// TINYCC  lib/va_list.c, included here to make compilation work.

enum __va_arg_type { __va_gen_reg, __va_float_reg, __va_stack };

extern void *memcpy(void *dest, const void *src, unsigned long n);

void *
__va_arg(__builtin_va_list ap, int arg_type, int size, int align)
{
  size = (size + 7) & ~7;
  align = (align + 7) & ~7;
  switch ((enum __va_arg_type)arg_type) {
  case __va_gen_reg:
    if (ap->gp_offset + size <= 48) {
      ap->gp_offset += size;
      return ap->reg_save_area + ap->gp_offset - size;
    }
    goto use_overflow_area;

  case __va_float_reg:
    if (ap->fp_offset < 128 + 48) {
      ap->fp_offset += 16;
      if (size == 8)
        return ap->reg_save_area + ap->fp_offset - 16;
      if (ap->fp_offset < 128 + 48) {
        memcpy(ap->reg_save_area + ap->fp_offset - 8,
               ap->reg_save_area + ap->fp_offset, 8);
        ap->fp_offset += 16;
        return ap->reg_save_area + ap->fp_offset - 32;
      }
    }
    goto use_overflow_area;

  case __va_stack:
  use_overflow_area:
    ap->overflow_arg_area += size;
    ap->overflow_arg_area =
        (char *)((long long)(ap->overflow_arg_area + align - 1) & -align);
    return ap->overflow_arg_area - size;
  default: return 0;
  }
}
