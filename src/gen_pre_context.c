#define USE_BASIC_CONFIG 1

#include "basic-config.h"
/*
 * basic-config.h would undef USE_ENDOMORPHISM in any case, hence
 * we need to restore it if present.
 */
#ifdef CALCULATE_ENDOMORPHISM
#define USE_ENDOMORPHISM 1
#endif

#include "include/secp256k1.h"
#include "field_impl.h"
#include "scalar_impl.h"
#include "group_impl.h"
#include "ecmult_impl.h"
#include "scratch_impl.h"

#define PRE_SIZE ECMULT_TABLE_SIZE(WINDOW_G)
#ifdef USE_ENDOMORPHISM
#define PRE128_SIZE ECMULT_TABLE_SIZE(WINDOW_G)
#endif

static void default_error_callback_fn(const char* str, void* data) {
  (void)data;
  fprintf(stderr, "[libsecp256k1] internal consistency check failed: %s\n", str);
  abort();
}

static const secp256k1_callback default_error_callback = {
  default_error_callback_fn,
  NULL
};

int main(int argc, char **argv) {
  secp256k1_ecmult_context ctx;
  int i;
  FILE* fp;

  (void)argc;
  (void)argv;

  fp = fopen("src/ecmult_static_pre_context.h","w");
  if (fp == NULL) {
    fprintf(stderr, "Could not open src/ecmult_static_pre_context.h for writing!\n");
    return -1;
  }

  fprintf(fp, "#ifndef _SECP256K1_ECMULT_STATIC_PRE_CONTEXT_\n");
  fprintf(fp, "#define _SECP256K1_ECMULT_STATIC_PRE_CONTEXT_\n");
  fprintf(fp, "#include \"src/group.h\"\n");
  fprintf(fp, "#define SC SECP256K1_GE_STORAGE_CONST\n");

  fprintf(fp, "static const secp256k1_ge_storage secp256k1_ecmult_static_pre_context[%d] = {\n", PRE_SIZE);

  secp256k1_ecmult_context_init(&ctx);
  secp256k1_ecmult_context_build(&ctx, &default_error_callback);
  for(i = 0; i != PRE_SIZE; i++) {
    fprintf(fp, "    SC(%uu, %uu, %uu, %uu, %uu, %uu, %uu, %uu, "
            "%uu, %uu, %uu, %uu, %uu, %uu, %uu, %uu)",
            SECP256K1_GE_STORAGE_CONST_GET((*ctx.pre_g)[i]));
    if (i != PRE_SIZE - 1) {
      fprintf(fp, ",\n");
    } else {
      fprintf(fp, "\n");
    }
  }
  fprintf(fp,"};\n");

#ifdef USE_ENDOMORPHISM
  fprintf(fp, "static const secp256k1_ge_storage secp256k1_ecmult_static_pre128_context[%d] = {\n", PRE128_SIZE);

  secp256k1_ecmult_context_init(&ctx);
  secp256k1_ecmult_context_build(&ctx, &default_error_callback);
  for(i = 0; i != PRE128_SIZE; i++) {
    fprintf(fp, "    SC(%uu, %uu, %uu, %uu, %uu, %uu, %uu, %uu, "
            "%uu, %uu, %uu, %uu, %uu, %uu, %uu, %uu)",
            SECP256K1_GE_STORAGE_CONST_GET((*ctx.pre_g_128)[i]));
    if (i != PRE128_SIZE - 1) {
      fprintf(fp, ",\n");
    } else {
      fprintf(fp, "\n");
    }
  }
  fprintf(fp,"};\n");
#endif

  secp256k1_ecmult_context_clear(&ctx);

  fprintf(fp, "#undef SC\n");
  fprintf(fp, "#endif\n");
  fclose(fp);

  return 0;
}
