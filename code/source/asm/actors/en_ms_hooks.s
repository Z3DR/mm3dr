.arm
.text

.global hook_BeanDaddyCheckExtData
hook_BeanDaddyCheckExtData:
  push {r0-r12,lr}
  bl En_Ms_CheckBeanExtData
  cmp r0,#0x0
  pop {r0-r12,lr}
  bx lr

.global hook_BeanDaddyCustomSoldOutText
hook_BeanDaddyCustomSoldOutText:
  push {r0-r12,lr}
  bl En_Ms_CheckBeanInventory
  cmp r0,#0x0
  pop {r0-r12,lr}
  beq beanDaddyDeny
  cmp r0, #0x14
  bx lr
beanDaddyDeny:
  b 0x2B9A10