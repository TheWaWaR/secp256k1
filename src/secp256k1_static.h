#ifndef SECP256K1_STATIC_H
#define SECP256K1_STATIC_H

#ifndef HAVE_CONFIG_H
/* So we can use it elsewhere */
#define HAVE_CONFIG_H
#endif

#include "include/secp256k1.h"
#include "util.h"
#include "ecmult.h"
#include "ecmult_gen.h"

struct secp256k1_context_struct {
    secp256k1_ecmult_context ecmult_ctx;
    secp256k1_ecmult_gen_context ecmult_gen_ctx;
    secp256k1_callback illegal_callback;
    secp256k1_callback error_callback;
};

#endif /* SECP256K1_STATIC_H */
