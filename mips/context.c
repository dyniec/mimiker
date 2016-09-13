#include <stdc.h>
#include <context.h>

extern void kernel_exit();

void ctx_init(ctx_t *ctx, void (*target)(), void *sp) {
  register void *gp asm("$gp");

  bzero(ctx, sizeof(ctx_t));

  ctx->reg[REG_PC] = (reg_t)target;
  ctx->reg[REG_GP] = (reg_t)gp;
  ctx->reg[REG_SP] = (reg_t)sp;
  ctx->reg[REG_RA] = (reg_t)kernel_exit;
  ctx->reg[REG_SR] = (reg_t)mips32_get_c0(C0_STATUS);
}