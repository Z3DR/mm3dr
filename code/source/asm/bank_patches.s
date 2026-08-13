.arm


.section .patch_BankTierTwoCurrent
.global patch_BankTierTwoCurrent
patch_BankTierTwoCurrent:
    cmp r2,#0x1F4

.section .patch_BankTierTwoPrevious
.global patch_BankTierTwoPrevious
patch_BankTierTwoPrevious:
    cmp r1,#0x1F4

.section .patch_BankTierThreeThreshold
.global patch_BankTierThreeThreshold
patch_BankTierThreeThreshold:
    mov r12,#0x3E8