/**
  ******************************************************************************
  * @file    audio.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-12-08T02:21:15+0600
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "audio.h"
#include "audio_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_audio
 
#undef AI_AUDIO_MODEL_SIGNATURE
#define AI_AUDIO_MODEL_SIGNATURE     "0xd09e5c1642b73dfcd50443a4a39e5e3b"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2025-12-08T02:21:15+0600"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_AUDIO_N_BATCHES
#define AI_AUDIO_N_BATCHES         (1)

static ai_ptr g_audio_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_audio_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_input_layer0_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 40, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 200, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 100, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 5, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  nl_4_output_array, AI_ARRAY_FORMAT_S8|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 5, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4000, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 100, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 20000, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 200, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 20000, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 100, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 500, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 5, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  gemm_0_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 540, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 1100, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 700, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 125, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  nl_4_scratch0_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 7, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(1.511865258216858f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_0_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 100,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0019389591179788113f, 0.0027184945065528154f, 0.0028148931451141834f, 0.0016709677875041962f, 0.001559039345011115f, 0.002459317911416292f, 0.0037246951833367348f, 0.0030628207605332136f, 0.002695616567507386f, 0.0015116228023543954f, 0.0016186407301574945f, 0.0026495475322008133f, 0.0015455883694812655f, 0.0033513547386974096f, 0.003081946400925517f, 0.0016275689704343677f, 0.001596044166944921f, 0.0024507928173989058f, 0.0030466679017990828f, 0.0023444495163857937f, 0.0015691634034737945f, 0.0016605081036686897f, 0.003647408215329051f, 0.0016394478734582663f, 0.002362598432227969f, 0.002558361738920212f, 0.0032326877117156982f, 0.0016294174129143357f, 0.00252571445889771f, 0.003787172958254814f, 0.00216555199585855f, 0.0014772409340366721f, 0.0020598797127604485f, 0.002003422239795327f, 0.0019099389901384711f, 0.002797884400933981f, 0.0033151067327708006f, 0.002682494232431054f, 0.003532234113663435f, 0.0015773264458402991f, 0.002612793119624257f, 0.0015809675678610802f, 0.0016229241155087948f, 0.002868861425668001f, 0.002538166707381606f, 0.0016020259354263544f, 0.0024802186526358128f, 0.0015091338427737355f, 0.001601811614818871f, 0.002282226225361228f, 0.0015180809423327446f, 0.0021396789234131575f, 0.0015942431055009365f, 0.003417810657992959f, 0.0028500952757894993f, 0.0016273598885163665f, 0.002756852423772216f, 0.0029349722899496555f, 0.0015443506417796016f, 0.0037481295876204967f, 0.0015542323235422373f, 0.0026728608645498753f, 0.0015335837379097939f, 0.002954989904537797f, 0.002579917199909687f, 0.0015982694458216429f, 0.0015970274107530713f, 0.0015473505482077599f, 0.0024929179344326258f, 0.0032816072925925255f, 0.003024812089279294f, 0.0025793062523007393f, 0.00321691925637424f, 0.0022007108200341463f, 0.0016213865019381046f, 0.0028649556916207075f, 0.0015605947701260448f, 0.0015904275933280587f, 0.0016265932936221361f, 0.0022370703518390656f, 0.0033757006749510765f, 0.0016031216364353895f, 0.002684660954400897f, 0.002709353808313608f, 0.00265546515583992f, 0.00161966762971133f, 0.0015781262191012502f, 0.0031557625625282526f, 0.001629265840165317f, 0.003634990891441703f, 0.0030097824055701494f, 0.003604907775297761f, 0.002006043214350939f, 0.0016172233736142516f, 0.0015601662453263998f, 0.0027266708202660084f, 0.001616353983990848f, 0.0036614800337702036f, 0.0024082823656499386f, 0.0015810675686225295f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_1_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(1.2150719165802002f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_1_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 200,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0013823171611875296f, 0.001766841858625412f, 0.002297364640980959f, 0.002251175930723548f, 0.0016504869563505054f, 0.0013219288084656f, 0.002223125658929348f, 0.0011830537114292383f, 0.001230756868608296f, 0.0017168568447232246f, 0.001321295858360827f, 0.0017733278218656778f, 0.0014399890787899494f, 0.0012734818737953901f, 0.0012742652324959636f, 0.00120398192666471f, 0.00244744960218668f, 0.001105510164052248f, 0.0012677572667598724f, 0.0011750252451747656f, 0.0016534932656213641f, 0.0014132366050034761f, 0.0011740683112293482f, 0.002131690504029393f, 0.0014766172971576452f, 0.0014521226985380054f, 0.001996639184653759f, 0.0019120712531730533f, 0.0014939720276743174f, 0.001307062921114266f, 0.0020151901990175247f, 0.001211377209983766f, 0.0015213449951261282f, 0.002100688638165593f, 0.0012077440042048693f, 0.001737917889840901f, 0.001960882917046547f, 0.001487592002376914f, 0.0014969439944252372f, 0.0018180516781285405f, 0.0012620729394257069f, 0.0012295300839468837f, 0.0015691040316596627f, 0.0015467703342437744f, 0.001500216661952436f, 0.001850020489655435f, 0.0022240947000682354f, 0.0017066200962290168f, 0.002298687119036913f, 0.0013633641647174954f, 0.0017694927519187331f, 0.0019658617675304413f, 0.0017086247680708766f, 0.0018093314720317721f, 0.001147085102275014f, 0.0017979854019358754f, 0.002429378917440772f, 0.0021622953936457634f, 0.0013132552849128842f, 0.0011134911328554153f, 0.0017330178525298834f, 0.0012540622847154737f, 0.002736901631578803f, 0.002065716776996851f, 0.0019482841016724706f, 0.0017910184105858207f, 0.001969067845493555f, 0.0018425461603328586f, 0.00142346927896142f, 0.001968445722013712f, 0.001747424015775323f, 0.001848268206231296f, 0.0012394407531246543f, 0.0013601426035165787f, 0.0013178498484194279f, 0.0017286974471062422f, 0.0014848739374428988f, 0.001985236071050167f, 0.0016684805741533637f, 0.001538938726298511f, 0.0013544592075049877f, 0.001988036325201392f, 0.001689088880084455f, 0.0014556498499587178f, 0.0013866120716556907f, 0.0011676071444526315f, 0.0014621332520619035f, 0.0018120683962479234f, 0.0018242632504552603f, 0.0016815521521493793f, 0.001173366792500019f, 0.001390959252603352f, 0.0015288114082068205f, 0.0021280075889080763f, 0.0013207029551267624f, 0.0016338792629539967f, 0.001835135160945356f, 0.0018299041548743844f, 0.001758531085215509f, 0.00204067281447351f, 0.0012564259814098477f, 0.0011301139602437615f, 0.001572572742588818f, 0.0013474470470100641f, 0.0012971011456102133f, 0.0017125422600656748f, 0.0015998266171664f, 0.001253239344805479f, 0.001911366474814713f, 0.0013162738177925348f, 0.0014031862374395132f, 0.0015265002148225904f, 0.0015513411490246654f, 0.0021820454858243465f, 0.0022917294409126043f, 0.002327912487089634f, 0.0013421924086287618f, 0.0013498731423169374f, 0.0017356250900775194f, 0.0015253444435074925f, 0.001445118454284966f, 0.0014164610765874386f, 0.0016981175867840648f, 0.002126359846442938f, 0.0012906994670629501f, 0.0018862382275983691f, 0.0012074752012267709f, 0.0014533975627273321f, 0.002037908649072051f, 0.001712506404146552f, 0.001575588365085423f, 0.0026553322095423937f, 0.0015077813295647502f, 0.0018288939027115703f, 0.0012618647888302803f, 0.0014410791918635368f, 0.001237464020960033f, 0.0013433079002425075f, 0.0012343626003712416f, 0.0013331357622519135f, 0.002221114467829466f, 0.0016093817539513111f, 0.002520404290407896f, 0.0012379243271425366f, 0.001849298132583499f, 0.0018240928184241056f, 0.0015519640874117613f, 0.001858802861534059f, 0.0019453864078968763f, 0.001609495491720736f, 0.001739490544423461f, 0.001523033482953906f, 0.0017871027812361717f, 0.0014255085261538625f, 0.0016853458946570754f, 0.0013326701009646058f, 0.0012420088751241565f, 0.002228205092251301f, 0.0013192857149988413f, 0.0013169368030503392f, 0.0014320649206638336f, 0.0016831024549901485f, 0.001233062706887722f, 0.0017489491729065776f, 0.001396017731167376f, 0.001628620782867074f, 0.0012609813129529357f, 0.0016476892633363605f, 0.002121482277289033f, 0.0022335879039019346f, 0.0018546944484114647f, 0.001901593990623951f, 0.0013937613693997264f, 0.0017439218936488032f, 0.0020116770174354315f, 0.0015073595568537712f, 0.0016647538868710399f, 0.0018515607807785273f, 0.0016884123906493187f, 0.001309820101596415f, 0.0015789641765877604f, 0.0014721006155014038f, 0.0018904110183939338f, 0.001350101549178362f, 0.0017603241140022874f, 0.0014248808147385716f, 0.0016571233281865716f, 0.0013146682176738977f, 0.0016014089342206717f, 0.0011934222420677543f, 0.0019712336361408234f, 0.0014450756134465337f, 0.0015140526229515672f, 0.0026441123336553574f, 0.002100821118801832f, 0.001797215430997312f, 0.0013908793916925788f, 0.0015364912105724216f, 0.0012884183088317513f, 0.0017827135743573308f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_2_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(1.2782856225967407f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_2_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 100,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0015516112325713038f, 0.0019885702058672905f, 0.0020633332896977663f, 0.0017938490491360426f, 0.0014402015367522836f, 0.001639929017983377f, 0.0016838285373523831f, 0.0018174853175878525f, 0.001684569288045168f, 0.0016318567795678973f, 0.0017957886448130012f, 0.0014707293594256043f, 0.0014834596076980233f, 0.0014761693309992552f, 0.0019442648626863956f, 0.0013731314102187753f, 0.0021699636708945036f, 0.0014713421696797013f, 0.001703919260762632f, 0.0022143202368170023f, 0.0013420898467302322f, 0.001350665814243257f, 0.001507079810835421f, 0.0015205160016193986f, 0.0017719323514029384f, 0.0015180357731878757f, 0.002356390468776226f, 0.001822201767936349f, 0.0011929543688893318f, 0.0015781299443915486f, 0.0014802885707467794f, 0.0015547015937045217f, 0.001690862700343132f, 0.0015881428262218833f, 0.0015852098586037755f, 0.0016790763475000858f, 0.0016428993549197912f, 0.0014620533911511302f, 0.0026415113825351f, 0.0018144769128412008f, 0.001514748902991414f, 0.0013194461353123188f, 0.0015191722195595503f, 0.0018664707895368338f, 0.0016527499537914991f, 0.0017321689520031214f, 0.0015575926518067718f, 0.0015514747938141227f, 0.0017914380878210068f, 0.0018442274304106832f, 0.0016927312826737761f, 0.0018630045233294368f, 0.001981968991458416f, 0.0018295740010216832f, 0.0013428552774712443f, 0.0019361746963113546f, 0.0015121072065085173f, 0.0016109751304611564f, 0.0014013919280841947f, 0.0013096882030367851f, 0.0021153732668608427f, 0.0016676767263561487f, 0.0016648289747536182f, 0.0015992425614967942f, 0.0018581687472760677f, 0.0016533737070858479f, 0.0013463898794725537f, 0.001461432664655149f, 0.0013008567038923502f, 0.0019053651485592127f, 0.0017765189986675978f, 0.0015153762651607394f, 0.0016583099495619535f, 0.0021151849068701267f, 0.001325414516031742f, 0.0017939971294254065f, 0.001652076025493443f, 0.0013057010946795344f, 0.001556292176246643f, 0.0014892667531967163f, 0.0016191612230613828f, 0.002422324614599347f, 0.001969648525118828f, 0.0018112176330760121f, 0.0014987483154982328f, 0.0016897087916731834f, 0.0017986948369070888f, 0.0017485445132479072f, 0.002004442736506462f, 0.0015898598358035088f, 0.0013507119147107005f, 0.001994884107261896f, 0.0013029451947659254f, 0.002045438392087817f, 0.001682389760389924f, 0.0016952044097706676f, 0.0015341243706643581f, 0.001666769734583795f, 0.0017367737600579858f, 0.0014758703764528036f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_3_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(2.8898167610168457f),
    AI_PACK_INTQ_ZP(-43)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_3_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 5,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0018970005912706256f, 0.0019566616974771023f, 0.0020352238789200783f, 0.001767755951732397f, 0.0019348387140780687f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(nl_4_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00390625f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(serving_default_input_layer0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(2.7867205142974854f),
    AI_PACK_INTQ_ZP(99)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_bias, AI_STATIC,
  0, 0x0,
  AI_SHAPE_INIT(4, 1, 100, 1, 1), AI_STRIDE_INIT(4, 4, 4, 400, 400),
  1, &gemm_0_bias_array, NULL)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_output, AI_STATIC,
  1, 0x1,
  AI_SHAPE_INIT(4, 1, 100, 1, 1), AI_STRIDE_INIT(4, 1, 1, 100, 100),
  1, &gemm_0_output_array, &gemm_0_output_array_intq)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_scratch0, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 540, 1, 1), AI_STRIDE_INIT(4, 2, 2, 1080, 1080),
  1, &gemm_0_scratch0_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  gemm_0_weights, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 40, 100, 1, 1), AI_STRIDE_INIT(4, 1, 40, 4000, 4000),
  1, &gemm_0_weights_array, &gemm_0_weights_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_bias, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 4, 4, 800, 800),
  1, &gemm_1_bias_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_output, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 1, 200, 1, 1), AI_STRIDE_INIT(4, 1, 1, 200, 200),
  1, &gemm_1_output_array, &gemm_1_output_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_scratch0, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 1100, 1, 1), AI_STRIDE_INIT(4, 2, 2, 2200, 2200),
  1, &gemm_1_scratch0_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_weights, AI_STATIC,
  7, 0x1,
  AI_SHAPE_INIT(4, 100, 200, 1, 1), AI_STRIDE_INIT(4, 1, 100, 20000, 20000),
  1, &gemm_1_weights_array, &gemm_1_weights_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_bias, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 100, 1, 1), AI_STRIDE_INIT(4, 4, 4, 400, 400),
  1, &gemm_2_bias_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_output, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 1, 100, 1, 1), AI_STRIDE_INIT(4, 1, 1, 100, 100),
  1, &gemm_2_output_array, &gemm_2_output_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_scratch0, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 700, 1, 1), AI_STRIDE_INIT(4, 2, 2, 1400, 1400),
  1, &gemm_2_scratch0_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_weights, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 200, 100, 1, 1), AI_STRIDE_INIT(4, 1, 200, 20000, 20000),
  1, &gemm_2_weights_array, &gemm_2_weights_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_bias, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 4, 4, 20, 20),
  1, &gemm_3_bias_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_output, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5, 5),
  1, &gemm_3_output_array, &gemm_3_output_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_scratch0, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 125, 1, 1), AI_STRIDE_INIT(4, 2, 2, 250, 250),
  1, &gemm_3_scratch0_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_weights, AI_STATIC,
  15, 0x1,
  AI_SHAPE_INIT(4, 100, 5, 1, 1), AI_STRIDE_INIT(4, 1, 100, 500, 500),
  1, &gemm_3_weights_array, &gemm_3_weights_array_intq)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  nl_4_output, AI_STATIC,
  16, 0x1,
  AI_SHAPE_INIT(4, 1, 5, 1, 1), AI_STRIDE_INIT(4, 1, 1, 5, 5),
  1, &nl_4_output_array, &nl_4_output_array_intq)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  nl_4_scratch0, AI_STATIC,
  17, 0x0,
  AI_SHAPE_INIT(4, 1, 7, 1, 1), AI_STRIDE_INIT(4, 4, 4, 28, 28),
  1, &nl_4_scratch0_array, NULL)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_input_layer0_output, AI_STATIC,
  18, 0x1,
  AI_SHAPE_INIT(4, 1, 40, 1, 1), AI_STRIDE_INIT(4, 1, 1, 40, 40),
  1, &serving_default_input_layer0_output_array, &serving_default_input_layer0_output_array_intq)



/**  Layer declarations section  **********************************************/



AI_STATIC_CONST ai_i32 nl_4_nl_params_data[] = { 1551458560, 28, -7 };
AI_ARRAY_OBJ_DECLARE(
    nl_4_nl_params, AI_ARRAY_FORMAT_S32,
    nl_4_nl_params_data, nl_4_nl_params_data, 3, AI_STATIC_CONST)
AI_TENSOR_CHAIN_OBJ_DECLARE(
  nl_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_4_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &nl_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  nl_4_layer, 4,
  SM_TYPE, 0x0, NULL,
  sm, forward_sm_integer,
  &nl_4_chain,
  NULL, &nl_4_layer, AI_STATIC, 
  .nl_params = &nl_4_nl_params, 
  .axis = AI_SHAPE_CHANNEL, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_3_weights, &gemm_3_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_3_layer, 3,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_3_chain,
  NULL, &nl_4_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_2_weights, &gemm_2_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_2_layer, 2,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_2_chain,
  NULL, &gemm_3_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_1_weights, &gemm_1_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_1_layer, 1,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_1_chain,
  NULL, &gemm_2_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_input_layer0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_0_weights, &gemm_0_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_0_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_0_layer, 0,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_0_chain,
  NULL, &gemm_1_layer, AI_STATIC, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 46120, 1, 1),
    46120, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2500, 1, 1),
    2500, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_IN_NUM, &serving_default_input_layer0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_OUT_NUM, &nl_4_output),
  &gemm_0_layer, 0x7cb30d1a, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 46120, 1, 1),
      46120, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 2500, 1, 1),
      2500, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_IN_NUM, &serving_default_input_layer0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AUDIO_OUT_NUM, &nl_4_output),
  &gemm_0_layer, 0x7cb30d1a, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool audio_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_audio_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_input_layer0_output_array.data = AI_PTR(g_audio_activations_map[0] + 1080);
    serving_default_input_layer0_output_array.data_start = AI_PTR(g_audio_activations_map[0] + 1080);
    gemm_0_scratch0_array.data = AI_PTR(g_audio_activations_map[0] + 1120);
    gemm_0_scratch0_array.data_start = AI_PTR(g_audio_activations_map[0] + 1120);
    gemm_0_output_array.data = AI_PTR(g_audio_activations_map[0] + 2200);
    gemm_0_output_array.data_start = AI_PTR(g_audio_activations_map[0] + 2200);
    gemm_1_scratch0_array.data = AI_PTR(g_audio_activations_map[0] + 0);
    gemm_1_scratch0_array.data_start = AI_PTR(g_audio_activations_map[0] + 0);
    gemm_1_output_array.data = AI_PTR(g_audio_activations_map[0] + 2300);
    gemm_1_output_array.data_start = AI_PTR(g_audio_activations_map[0] + 2300);
    gemm_2_scratch0_array.data = AI_PTR(g_audio_activations_map[0] + 0);
    gemm_2_scratch0_array.data_start = AI_PTR(g_audio_activations_map[0] + 0);
    gemm_2_output_array.data = AI_PTR(g_audio_activations_map[0] + 1400);
    gemm_2_output_array.data_start = AI_PTR(g_audio_activations_map[0] + 1400);
    gemm_3_scratch0_array.data = AI_PTR(g_audio_activations_map[0] + 0);
    gemm_3_scratch0_array.data_start = AI_PTR(g_audio_activations_map[0] + 0);
    gemm_3_output_array.data = AI_PTR(g_audio_activations_map[0] + 252);
    gemm_3_output_array.data_start = AI_PTR(g_audio_activations_map[0] + 252);
    nl_4_scratch0_array.data = AI_PTR(g_audio_activations_map[0] + 0);
    nl_4_scratch0_array.data_start = AI_PTR(g_audio_activations_map[0] + 0);
    nl_4_output_array.data = AI_PTR(g_audio_activations_map[0] + 28);
    nl_4_output_array.data_start = AI_PTR(g_audio_activations_map[0] + 28);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool audio_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_audio_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    gemm_0_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_0_weights_array.data = AI_PTR(g_audio_weights_map[0] + 0);
    gemm_0_weights_array.data_start = AI_PTR(g_audio_weights_map[0] + 0);
    gemm_0_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_0_bias_array.data = AI_PTR(g_audio_weights_map[0] + 4000);
    gemm_0_bias_array.data_start = AI_PTR(g_audio_weights_map[0] + 4000);
    gemm_1_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_1_weights_array.data = AI_PTR(g_audio_weights_map[0] + 4400);
    gemm_1_weights_array.data_start = AI_PTR(g_audio_weights_map[0] + 4400);
    gemm_1_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_1_bias_array.data = AI_PTR(g_audio_weights_map[0] + 24400);
    gemm_1_bias_array.data_start = AI_PTR(g_audio_weights_map[0] + 24400);
    gemm_2_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_2_weights_array.data = AI_PTR(g_audio_weights_map[0] + 25200);
    gemm_2_weights_array.data_start = AI_PTR(g_audio_weights_map[0] + 25200);
    gemm_2_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_2_bias_array.data = AI_PTR(g_audio_weights_map[0] + 45200);
    gemm_2_bias_array.data_start = AI_PTR(g_audio_weights_map[0] + 45200);
    gemm_3_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_3_weights_array.data = AI_PTR(g_audio_weights_map[0] + 45600);
    gemm_3_weights_array.data_start = AI_PTR(g_audio_weights_map[0] + 45600);
    gemm_3_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_3_bias_array.data = AI_PTR(g_audio_weights_map[0] + 46100);
    gemm_3_bias_array.data_start = AI_PTR(g_audio_weights_map[0] + 46100);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_audio_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_AUDIO_MODEL_NAME,
      .model_signature   = AI_AUDIO_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 44980,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x7cb30d1a,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_audio_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_AUDIO_MODEL_NAME,
      .model_signature   = AI_AUDIO_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 44980,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x7cb30d1a,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_audio_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_audio_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_audio_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_audio_create(network, AI_AUDIO_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_audio_data_params_get(&params) != true) {
    err = ai_audio_get_error(*network);
    return err;
  }
#if defined(AI_AUDIO_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_AUDIO_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_audio_init(*network, &params) != true) {
    err = ai_audio_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_audio_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_audio_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_audio_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_audio_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= audio_configure_weights(net_ctx, params);
  ok &= audio_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_audio_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_audio_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_AUDIO_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

