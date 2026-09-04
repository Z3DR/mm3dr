.arm

.section .patch_MusicOverridePlay
.global patch_MusicOverridePlay
patch_MusicOverridePlay:
  bl hook_MusicOverridePlay
