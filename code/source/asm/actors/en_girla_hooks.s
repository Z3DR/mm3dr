.arm
.text

@ hook body for patch_EnGirlA_Randomize (patch @ 0x2b8204). At the patch site r4 = EnGirlA actor,
@ r7 = GlobalContext. Run the shopsanity override, then replay the replaced instruction
@ (`ldr r0,[0x2b8270]` -> r0 = *(0x2b8270), the next_fn pointer) and rejoin the vanilla flow.
.global hook_EnGirlA_Randomize
hook_EnGirlA_Randomize:
    push {r0-r12, lr}
    cpy r0, r4
    cpy r1, r7
    bl EnGirlA_Randomize
    pop {r0-r12, lr}
    ldr r0, =0x2b8270 @ replay: r0 = *(0x2b8270)
    ldr r0, [r0]
    b 0x2b8208

.global hook_EnGirlA_AfterModelLoad
hook_EnGirlA_AfterModelLoad:
    push {r0-r12, lr}
    cpy r0, r4
    cpy r1, r7
    bl EnGirlA_AfterModelLoad
    pop {r0-r12, lr}
    ldmia sp!,{r4,r5,r6,r7,r8,r9,r10,pc} @ replayed original return