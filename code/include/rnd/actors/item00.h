#ifndef _RND_ACTORS_ITEM00_H_
#define _RND_ACTORS_ITEM00_H_

#include "game/actor.h"
#include "rnd/models.h"
#if defined ENABLE_DEBUG || defined DEBUG_PRINT
#include "common/debug.h"
extern "C" {
#include <3ds/svc.h>
}
#endif
namespace rnd {
  extern "C" {
  void SpawnItem00Model(game::act::Actor*);
  u8 DrawItem00Model(game::act::Actor*);
  }

  struct En_Item00 : public game::act::Actor {
    void* collectible_calc;
    u16 field_1FC;
    u16 field_1FE;
    u16 field_200;
    u16 field_202;
    u16 field_204;
    u16 field_206;
    float scale;
    game::CollisionInfoCylinder collider;
    u8 collected;
    u8 gap_265[11];
    void* skel_anime_model;
  };

  enum class Zelda2Keep : u16 {
    CMB_A_FIREBALL_MODEL = 0X00,             // actor_bomb/model/a_fireball_model.cmb
    CMB_ARROW_MODEL = 0X01,                  // arrow/model/arrow_model.cmb
    CMB_M_ARROW_MODELT_FIRE = 0X02,          // arrow_fire/model/m_arrow_modelT_fire.cmb
    CMB_M_ARROW_MODELT_ICE = 0X03,           // arrow_ice/model/m_arrow_modelT_ice.cmb
    CMB_M_ARROW_MODELT_LIGHT = 0X04,         // arrow_light/model/m_arrow_modelT_light.cmb
    CMB_BOM_BODY_MODEL = 0X05,               // bom/model/bom_body_model.cmb
    CMB_BOM_HANDLE_MODEL = 0X06,             // bom/model/bom_handle_model.cmb
    CMB_BB_MODEL_MODEL = 0X07,               // bom/model/bb_model_model.cmb
    CMB_BB_RELIEF_MODEL = 0X08,              // bom/model/bb_relief_model.cmb
    CMB_FIRE_LINE_POINT_START_MODEL = 0X09,  // bom/model/fire_line_point_start_model.cmb
    CMB_BOM_CHU_MODEL = 0X0A,                // bom_chu/model/bom_chu_model.cmb
    CMB_BOMB_FRAGMENT_MODEL = 0X0B,          // bomb_fragment/model/bomb_fragment_model.cmb
    CMB_EFC_CANDLE_MODELT = 0X0C,            // candle/model/efc_candle_modelT.cmb
    CMB_PLS_ZO_MODEL = 0X0D,                 // clink_zo/model/pls_zo_model.cmb
    CMB_DDG_FIRE = 0X0E,                     // ddg_fire/model/ddg_fire.cmb
    CMB_CLL_MODELS = 0X0F,                   // debug/model/cll_models.cmb
    CMB_DOOR_MODEL = 0X10,                   // door/model/door_model.cmb
    CMB_EFC_FLASH_MODELT = 0X11,             // efc_flash/model/efc_flash_modelT.cmb
    CMB_EFC_PIKA_MODEL = 0X12,               // efc_flash/model/efc_pika_model.cmb
    CMB_EFC_SPARK_MODEL = 0X13,              // efc_flash/model/efc_spark_model.cmb
    CMB_EFC_FLASH2_MODELT = 0X14,            // efc_flash/model/efc_flash2_modelT.cmb
    CMB_EFC_SWORD_TAME_MODELT = 0X15,        // efc_sword_kaiten/model/efc_sword_tame_modelT.cmb
    CMB_EFC_SWORD_TUKI_MODELT = 0X16,        // efc_sword_kaiten/model/efc_sword_tuki_modelT.cmb
    CMB_SWD_MODELT1 = 0X17,                  // efc_sword_kaiten/model/swd_modelT1.cmb
    CMB_SWD_MODELT2 = 0X18,                  // efc_sword_kaiten/model/swd_modelT2.cmb
    CMB_ELF_NEW_FLY = 0X19,                  // elf_new/model/elf_new_fly.cmb
    CMB_CHAT = 0X1A,                         // elf_new/model/chat.cmb
    CMB_LIGHT_MODEL = 0X1B,                  // elf_new/model/light_model.cmb
    CMB_POINT_LIGHT_INIT_MODEL = 0X1C,       // elf_new/model/point_light_init_model.cmb
    CMB_POINT_LIGHT_MODEL = 0X1D,            // elf_new/model/point_light_model.cmb
    CMB_STRAYFAIRY = 0X1E,                   // elf_org/model/strayfairy.cmb
    CMB_NUKEGARA_ANM = 0X1F,                 // eff_change2/model/nukegara_anm.cmb
    CMB_OB_F_FIRECIRCLE_MODELT = 0X20,       // f_circle/model/OB_F_firecircle_modelT.cmb
    CMB_EYE_NEWMOONLOD_MODEL = 0X21,         // fall/model/eye_newmoonlod_model.cmb
    CMB_NEWMOONLOD_MODEL = 0X22,             // fall/model/newmoonlod_model.cmb
    CMB_G_BLAST_MODEL = 0X23,                // g_blast/model/g_blast_model.cmb
    CMB_G_FIRE1_MODEL = 0X24,                // g_fire/model/g_fire1_model.cmb
    CMB_G_RIPPLE_MODEL = 0X25,               // g_ripple/model/g_ripple_model.cmb
    CMB_G_SPARK_MODEL = 0X26,                // g_spark/model/g_spark_model.cmb
    CMB_G_SPLASH2_MODEL = 0X27,              // g_splash/model/g_splash2_model.cmb
    CMB_G_STONE1_MODEL = 0X28,               // g_stone1/model/g_stone1_model.cmb
    CMB_G_THUNDER_MODEL = 0X29,              // g_thunder/model/g_thunder_model.cmb
    CMB_G_VANISH1_MODEL = 0X2A,              // g_vanish1/model/g_vanish1_model.cmb
    CMB_PGS_ZO_MODEL = 0X2B,                 // goron_zo/model/pgs_zo_model.cmb
    CMB_HOLL = 0X2C,                         // holl/model/holl.cmb
    CMB_OPAQUE_HOLL = 0X2D,                  // holl/model/opaque_holl.cmb
    CMB_ICE_SMOKE_MODELT = 0X2E,             // ice/model/ice_smoke_modelT.cmb
    CMB_ICE_PIECE_MODELT = 0X2F,             // ice/model/ice_piece_modelT.cmb
    CMB_ICE_PIECE2_MODELT = 0X30,            // ice/model/ice_piece2_modelT.cmb
    CMB_LINK_ICE_MODELT = 0X31,              // ice/model/link_ice_modelT.cmb
    CMB_INSECT_MOVE = 0X32,                  // insect/model/insect_move.cmb
    CMB_LENSFLARE2_MODEL = 0X33,             // kusa/model/lensflare2_model.cmb
    CMB_LENSFLARE3_MODEL = 0X34,             // kusa/model/lensflare3_model.cmb
    CMB_LC_STICK_MODEL = 0X35,               // lc_stick/model/lc_stick_model.cmb
    CMB_LINK_FIRE = 0X36,                    // link_fire/model/link_fire.cmb
    CMB_MASK_ZORA = 0X37,                    // mask/model/mask_zora.cmb
    CMB_MASK_TRUTH = 0X38,                   // mask/model/mask_truth.cmb
    CMB_MASK_STONE = 0X39,                   // mask/model/mask_stone.cmb
    CMB_MASK_ROMANI = 0X3A,                  // mask/model/mask_romani.cmb
    CMB_MASK_RABBIT = 0X3B,                  // mask/model/mask_rabbit.cmb
    CMB_MASK_POSTHAT = 0X3C,                 // mask/model/mask_posthat.cmb
    CMB_MASK_PIG = 0X3D,                     // mask/model/mask_pig.cmb
    CMB_MASK_NIGHTWALK = 0X3E,               // mask/model/mask_nightwalk.cmb
    CMB_MASK_KEATON = 0X3F,                  // mask/model/mask_keaton.cmb
    CMB_MASK_KAFAI = 0X40,                   // mask/model/mask_kafai.cmb
    CMB_MASK_GORON = 0X41,                   // mask/model/mask_goron.cmb
    CMB_MASK_GIBUDO = 0X42,                  // mask/model/mask_gibudo.cmb
    CMB_MASK_GIANT = 0X43,                   // mask/model/mask_giant.cmb
    CMB_MASK_FROG = 0X44,                    // mask/model/mask_frog.cmb
    CMB_MASK_FAIRY = 0X45,                   // mask/model/mask_fairy.cmb
    CMB_MASK_DEKNUTS2 = 0X46,                // mask/model/mask_deknuts2.cmb
    CMB_MASK_DEKNUTS = 0X47,                 // mask/model/mask_deknuts.cmb
    CMB_MASK_COUPLE = 0X48,                  // mask/model/mask_couple.cmb
    CMB_MASK_CHAIRMAN = 0X49,                // mask/model/mask_chairman.cmb
    CMB_MASK_CAPTAIN = 0X4A,                 // mask/model/mask_captain.cmb
    CMB_MASK_CAMARO = 0X4B,                  // mask/model/mask_camaro.cmb
    CMB_MASK_BREMEN = 0X4C,                  // mask/model/mask_bremen.cmb
    CMB_MASK_BOMB = 0X4D,                    // mask/model/mask_bomb.cmb
    CMB_MASK_GARO = 0X4E,                    // mask/model/mask_garo.cmb
    CMB_DNP_ALL_MODEL = 0X4F,                // MOGURI2/model/dnp_all_model.cmb
    CMB_DNZ_ALL_MODEL = 0X50,                // MOGURI2/model/dnz_all_model.cmb
    CMB_MOON_MODEL = 0X51,                   // moon/model/moon_model.cmb
    CMB_MSA_EFX_MODEL = 0X52,                // msa_efx/model/msa_efx_model.cmb
    CMB_Z2_16_NIKU_MODEL = 0X53,             // nikuc/model/z2_16_niku_model.cmb
    CMB_PNS_ZO_MODEL = 0X54,                 // nuts_zo/model/pns_zo_model.cmb
    CMB_KEEP_DN_BALL_MODEL = 0X55,           // nutsball/model/keep_dn_ball_model.cmb
    CMB_FIELD_KABU_MODEL = 0X56,             // objects/model/field_kabu_model.cmb
    CMB_FIELD_KUSA_MODEL = 0X57,             // objects/model/field_kusa_model.cmb
    CMB_GI_HP_MODELT = 0X58,                 // objects/model/gi_hp_modelT.cmb
    CMB_ITEM_MODEL = 0X59,                   // objects/model/item_model.cmb
    CMB_KANBAN1_MODEL = 0X5A,                // objects/model/kanban1_model.cmb
    CMB_KANBAN2_MODEL = 0X5B,                // objects/model/kanban2_model.cmb
    CMB_KIRENAI_KANBAN_01 = 0X5C,            // objects/model/kirenai_kanban_01.cmb
    CMB_RUPEE_MODEL = 0X5D,                  // objects/model/rupee_model.cmb
    CMB_IWAB_CUBE2_MODEL = 0X5E,             // prs/model/iwaB_cube2_model.cmb
    CMB_PRS = 0X5F,                          // prs/model/prs.cmb
    CMB_PN_TAMA_MODEL = 0X60,                // pz/model/pn_tama_model.cmb
    CMB_NUTS_SOAP = 0X61,                    // pz/model/nuts_soap.cmb
    CMB_LZ_LCUTTER_MODEL = 0X62,             // pz_cutter/model/lz_Lcutter_model.cmb
    CMB_LZ_RCUTTER_MODEL = 0X63,             // pz_cutter/model/lz_Rcutter_model.cmb
    CMB_AME_MODEL = 0X64,                    // rain/model/ame_model.cmb
    CMB_SBN_KABE_MODEL = 0X65,               // sbn2/model/sbn_kabe_model.cmb
    CMB_SBN_YUKA_MODEL = 0X66,               // sbn2/model/sbn_yuka_model.cmb
    CMB_A1_AC1T_MODEL = 0X67,                // shadow/model/a1_AC1T_model.cmb
    CMB_FOOT_SHADOW_MODEL = 0X68,            // shadow/model/foot_shadow_model.cmb
    CMB_SHADOW_H_MODEL = 0X69,               // shadow/model/shadow_h_model.cmb
    CMB_SHADOW_MODEL = 0X6A,                 // shadow/model/shadow_model.cmb
    CMB_SHADOW_B_MODEL = 0X6B,               // shadow/model/shadow_b_model.cmb
    CMB_N_DOOR_MODEL = 0X6C,                 // shutter/model/n_door_model.cmb
    CMB_TETUGOUSI_MODELT = 0X6D,             // shutter/model/tetugousi_modelT.cmb
    CMB_MONONOKE_POLYGON_MODEL = 0X6E,       // sun/model/mononoke_polygon_model.cmb
    CMB_SUN_MODEL = 0X6F,                    // sun/model/sun_model.cmb
    CMB_MARK_MODEL = 0X70,                   // target/model/mark_model.cmb
    CMB_MARK_DEPTH_MODEL = 0X71,             // target/model/mark_depth_model.cmb
    CMB_TARGET_MODEL = 0X72,                 // target/model/target_model.cmb
    CMB_TARGET_DEPTH_MODEL = 0X73,           // target/model/target_depth_model.cmb
    CMB_DISK_MODELT = 0X74,                  // time_warp/model/disk_modelT.cmb
    CMB_TORCH4_MODELT = 0X75,                // torch4/model/torch4_modelT.cmb
    CMB_TORCH5_MODELT = 0X76,                // torch4/model/torch5_modelT.cmb
    CMB_TORCH4_FIRE = 0X77,                  // torch4/model/torch4_fire.cmb
    CMB_TORCH4_KAJIYA_FIRE = 0X78,           // torch4/model/torch4_kajiya_fire.cmb
    CMB_MAP_UPDRAFT_MODELT = 0X79,           // updraft/model/map_updraft_modelT.cmb
    CMB_MAP_UPDRAFT_MODELT1 = 0X7A,          // updraft/model/map_updraft_modelT1.cmb
    CMB_BALL_MODEL = 0X7B,                   // wing_warp/model/ball_model.cmb
    CMB_FEATHER_MODEL = 0X7C,                // wing_warp/model/feather_model.cmb
    CMB_WIPE_MAKOTO = 0X7D,                  // wipe/model/wipe_makoto.cmb
    CMB_WIPE_MAKOTO_ALPHA_IN = 0X7E,         // wipe/model/wipe_makoto_alpha_in.cmb
    CMB_WIPE_MAKOTO_ALPHA_OUT = 0X7F,        // wipe/model/wipe_makoto_alpha_out.cmb
    CMB_PZS_ZO_MODEL = 0X80,                 // zora_zo/model/pzs_zo_model.cmb
    CMB_DEBUG_CUBE = 0X81,                   // debug_data/model/debug_cube.cmb
    CMB_DROP_GI_STICK = 0X82,                // item00/model/drop_gi_stick.cmb
    CMB_DROP_GI_ARROW_0 = 0X83,              // item00/model/drop_gi_arrow_0.cmb
    CMB_DROP_GI_ARROW_1 = 0X84,              // item00/model/drop_gi_arrow_1.cmb
    CMB_DROP_GI_ARROW_2 = 0X85,              // item00/model/drop_gi_arrow_2.cmb
    CMB_DROP_GI_BOMB_1 = 0X86,               // item00/model/drop_gi_bomb_1.cmb
    CMB_DROP_GI_HEART = 0X87,                // item00/model/drop_gi_heart.cmb
    CMB_DROP_GI_HEARTS_0 = 0X88,             // item00/model/drop_gi_hearts_0.cmb
    CMB_DROP_GI_HEARTS_1 = 0X89,             // item00/model/drop_gi_hearts_1.cmb
    CMB_DROP_GI_KEY = 0X8A,                  // item00/model/drop_gi_key.cmb
    CMB_DROP_GI_MAGICPOT_0 = 0X8B,           // item00/model/drop_gi_magicpot_0.cmb
    CMB_DROP_GI_MAGICPOT_1 = 0X8C,           // item00/model/drop_gi_magicpot_1.cmb
    CMB_DROP_GI_NUTS = 0X8D,                 // item00/model/drop_gi_nuts.cmb
    CMB_DROP_GI_OCARINA = 0X8E,              // item00/model/drop_gi_ocarina.cmb
    CMB_DROP_GI_RUPY = 0X8F,                 // item00/model/drop_gi_rupy.cmb
    CMB_GA_MODEL = 0X90,                     // tanron/model/ga_model.cmb
    CMB_FISH_02 = 0X91,                      // fish/model/fish_02.cmb
    CMB_STONE3_MODEL = 0X92,                 // ddan_hahen/model/stone3_model.cmb
    CMB_H_CURSOR = 0X93,                     // cursor/model/h_cursor.cmb
    CMB_WING_WARP = 0X94,                    // wing_warp/model/wing_warp.cmb
    CMB_EN_GAKUFU_DRAW_COMMON = 0X95,        // gakufu/model/en_gakufu_draw_common.cmb
    CMB_LN_HA_MODEL = 0X96,                  // nuts_ha/model/ln_ha_model.cmb
    CMB_MASK_DEMON = 0X97,                   // mask/model/mask_demon.cmb
    CMB_MASK_DEMON2 = 0X98,                  // mask/model/mask_demon2.cmb
    CMB_TORCH4_SOUL = 0X99,                  // torch4_soul/model/torch4_soul.cmb
    CMB_STEAM01 = 0X9A,                      // steam/model/steam01.cmb
    CMB_STEAM02 = 0X9B,                      // steam/model/steam02.cmb
    CMB_STEAM02_BOTTLE_HOTWATER = 0X9C,      // steam/model/steam02_bottle_hotwater.cmb
    CMB_STEAM03 = 0X9D,                      // steam/model/steam03.cmb
    CMB_MASK_GORON2 = 0X9E,                  // mask/model/mask_goron2.cmb
    CMB_MASK_ZORA2 = 0X9F,                   // mask/model/mask_zora2.cmb
    CMB_TORCH4_WATER01 = 0XA0,               // torch4_water/model/torch4_water01.cmb
    CMB_TORCH4_WATER02 = 0XA1,               // torch4_water/model/torch4_water02.cmb
    CMB_WE_WATER_MODELT = 0XA2,              // torch4_water/model/we_water_modelT.cmb
    CMB_SL_UFO_MODEL01 = 0XA3,               // ufo_light/model/sl_UFO_model01.cmb
    CMB_EFC_OCARINA_3_MODELT = 0XA4,         // efc_ocarina/model/efc_ocarina_3_modelT.cmb
    CMB_EFC_OCARINA_41_MODELT = 0XA5,        // efc_ocarina/model/efc_ocarina_41_modelT.cmb
    CMB_EFC_OCARINA_2_MODELTM = 0XA6,        // efc_ocarina/model/efc_ocarina_2_modelTm.cmb
    CMB_EFC_OCARINA_5_MODELT = 0XA7,         // efc_ocarina/model/efc_ocarina_5_modelT.cmb
    CMB_OCARINA_WING_MODELT = 0XA8,          // efc_ocarina/model/ocarina_wing_modelT.cmb
    CMB_G1T_MODEL = 0XA9,                    // efc_ocarina/model/g1T_model.cmb
    CMB_G1T_MODEL2 = 0XAA,                   // efc_ocarina/model/g1T_model2.cmb
    CMB_OCARINA_TIME_MODEL = 0XAB,           // efc_ocarina/model/ocarina_time_model.cmb
    CMB_OCARINA_EPONA_MODEL = 0XAC,          // efc_ocarina/model/ocarina_epona_model.cmb
    CMB_OCARINA_STORM_MODEL = 0XAD,          // efc_ocarina/model/ocarina_storm_model.cmb
    CMB_OCARINA_KAKASHI_MODEL = 0XAE,        // efc_ocarina/model/ocarina_kakashi_model.cmb
    CMB_CL_HOOKPIN_MODEL = 0XAF,             // hookpin/model/cl_hookpin_model.cmb
    CMB_CHAIN_MODEL = 0XB0,                  // hookpin/model/chain_model.cmb
  };
}  // namespace rnd
#endif