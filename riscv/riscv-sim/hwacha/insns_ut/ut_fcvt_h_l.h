require_xpr64;
require_fp;
softfloat_roundingMode = RM;
WRITE_HFRD(i64_to_f32(RS1));
set_fp_exceptions;
