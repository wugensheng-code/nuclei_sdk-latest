#include <stdint.h>
#include <stdio.h>

#include "nuclei_sdk_soc.h"
#include "ref_conv.h"
#include "riscv_math.h"

#ifndef READ_CYCLE
#define READ_CYCLE      __get_rv_cycle
#endif

static uint64_t enter_cycle;
static uint64_t exit_cycle;
static uint64_t start_cycle;
static uint64_t end_cycle;
static uint64_t cycle;
static uint64_t extra_cost = 0;
static uint32_t bench_ercd;

#define BENCH_TRST()

#define BENCH_INIT()                                                           \
    printf("Benchmark Initialized\n");                                         \
    BENCH_TRST();                                                              \
    start_cycle = READ_CYCLE();                                                \
    end_cycle = READ_CYCLE();                                                  \
    extra_cost = end_cycle - start_cycle;                                      \
    enter_cycle = READ_CYCLE();

#define BENCH_START(func)                                                      \
    bench_ercd = 0;                                                            \
    BENCH_TRST();                                                              \
    start_cycle = READ_CYCLE();

#define BENCH_END(func)                                                        \
    end_cycle = READ_CYCLE();                                                  \
    cycle = end_cycle - start_cycle - extra_cost;                              \
    printf("CSV, %s, %lu\n", #func, cycle);

#define BENCH_ERROR(func) bench_ercd = 1;
#define BENCH_STATUS(func)                                                     \
    if (bench_ercd) {                                                          \
        printf("ERROR, %s\n", #func);                                          \
    } else {                                                                   \
        printf("SUCCESS, %s\n", #func);                                        \
    }

#define BENCH_FINISH()                                                         \
    exit_cycle = READ_CYCLE();                                                 \
    cycle = exit_cycle - enter_cycle - extra_cost;                             \
    printf("CSV, BENCH END, %llu\n", cycle);

static float32_t test_conv_input_f32_A[300] = {
    0.240707035480160,    0.676122303863752,   0.289064571674477,
    0.671808165414215,    0.695140499551737,   0.0679927684700106,
    0.254790156597005,    0.224040030824219,   0.667832727013717,
    0.844392156527205,    0.344462411301042,   0.780519652731358,
    0.675332065747000,    0.00671531431847749, 0.602170487581795,
    0.386771194520985,    0.915991244131425,   0.00115105712910724,
    0.462449159242329,    0.424349039815375,   0.460916366028964,
    0.770159728608609,    0.322471807186779,   0.784739294760742,
    0.471357153710612,    0.0357627332691179,  0.175874415683531,
    0.721758033391102,    0.473485992965320,   0.152721200438232,
    0.341124607049109,    0.607389213768347,   0.191745255461798,
    0.738426839976942,    0.242849598318169,   0.917424342049382,
    0.269061586686018,    0.765500016621438,   0.188661976791491,
    0.287498173066131,    0.0911134636865350,  0.576209380663007,
    0.683363243294653,    0.546593114590323,   0.425728841871188,
    0.644442781431337,    0.647617630172684,   0.679016754093202,
    0.635786710514084,    0.945174113109401,   0.208934922426023,
    0.709281702710545,    0.236230576993797,   0.119396247797306,
    0.607303940685635,    0.450137696965896,   0.458725493648868,
    0.661944751905652,    0.770285514803660,   0.350218013441105,
    0.662009598359135,    0.416158589969797,   0.841929152691309,
    0.832916819075216,    0.256440992229147,   0.613460736812875,
    0.582249164527227,    0.540739337124410,   0.869941032358007,
    0.264779026475630,    0.318074075481059,   0.119214541054191,
    0.939829470344921,    0.645551874972524,   0.479463224948888,
    0.639316961040108,    0.544716110526763,   0.647311480293128,
    0.543885933999639,    0.721046620579811,   0.522495305777102,
    0.993704624120852,    0.218676632399634,   0.105798273250228,
    0.109697464523194,    0.0635913709751057,  0.404579995857626,
    0.448372912066495,    0.365816176838171,   0.763504640848813,
    0.627896379614169,    0.771980385554245,   0.932853570278820,
    0.972740854003014,    0.192028349427775,   0.138874202829155,
    0.696266337082995,    0.0938200267748656,  0.525404403859336,
    0.530344218392863,    0.861139811393332,   0.484853333552102,
    0.393456361215266,    0.671431139674026,   0.741257943454207,
    0.520052467390387,    0.347712671277525,   0.149997253831683,
    0.586092067231462,    0.262145317727807,   0.0444540922782385,
    0.754933267231179,    0.242785357820962,   0.442402313001943,
    0.687796085120107,    0.359228210401861,   0.736340074301202,
    0.394707475278763,    0.683415866967978,   0.704047430334266,
    0.442305413383371,    0.0195776235533187,  0.330857880214071,
    0.424309496833137,    0.270270423432065,   0.197053798095456,
    0.821721184961310,    0.429921409383266,   0.887770954256354,
    0.391182995461163,    0.769114387388296,   0.396791517013617,
    0.808514095887345,    0.755077099007084,   0.377395544835103,
    0.216018915961394,    0.790407217966913,   0.949303911849797,
    0.327565434075205,    0.671264370451740,   0.438644982586956,
    0.833500595588975,    0.768854252429615,   0.167253545494722,
    0.861980478702072,    0.989872153631504,   0.514423456505704,
    0.884281023126955,    0.588026055308498,   0.154752348656045,
    0.199862822857452,    0.406954837138907,   0.748705718215691,
    0.825583815786156,    0.789963029944531,   0.318524245398992,
    0.534064127370726,    0.0899506787705811,  0.111705744193203,
    0.136292548938299,    0.678652304800188,   0.495177019089661,
    0.189710406017580,    0.495005824990221,   0.147608221976689,
    0.0549741469061882,   0.850712674289007,   0.560559527354885,
    0.929608866756663,    0.696667200555228,   0.582790965175840,
    0.815397211477421,    0.879013904597178,   0.988911616079589,
    0.000522375356944771, 0.865438591013025,   0.612566469483999,
    0.989950205708831,    0.527680069338442,   0.479523385210219,
    0.801347605521952,    0.227842935706042,   0.498094291196390,
    0.900852488532005,    0.574661219130188,   0.845178185054037,
    0.738640291995402,    0.585987035826476,   0.246734525985975,
    0.666416217319468,    0.0834828136026227,  0.625959785171583,
    0.660944557947342,    0.729751855317221,   0.890752116325322,
    0.982303222883606,    0.769029085335896,   0.581446487875398,
    0.928313062314188,    0.580090365758442,   0.0169829383372613,
    0.120859571098558,    0.862710718699670,   0.484296511212103,
    0.844855674576263,    0.209405084020935,   0.552291341538775,
    0.629883385064421,    0.0319910157625669,  0.614713419117141,
    0.362411462273053,    0.0495325790420612,  0.489569989177322,
    0.192510396062075,    0.123083747545945,   0.205494170907680,
    0.146514910614890,    0.189072174472614,   0.0426524109111434,
    0.635197916859882,    0.281866855880430,   0.538596678045340,
    0.695163039444332,    0.499116013482590,   0.535801055751113,
    0.445183165296042,    0.123932277598070,   0.490357293468018,
    0.852998155340816,    0.873927405861733,   0.270294332292698,
    0.208461358751314,    0.564979570738201,   0.640311825162758,
    0.417028951642886,    0.205975515532243,   0.947933121293169,
    0.0820712070977259,   0.105709426581721,   0.142041121903998,
    0.166460440876421,    0.620958643935308,   0.573709764841198,
    0.0520778902858696,   0.931201384608250,   0.728661681678271,
    0.737841653797590,    0.0634045006928182,  0.860440563038232,
    0.934405118961213,    0.984398312240972,   0.858938816683866,
    0.785558989265031,    0.513377418587575,   0.177602460505865,
    0.398589496735843,    0.133931250987971,   0.0308895487449515,
    0.939141706069548,    0.301306064586392,   0.295533834475356,
    0.332936281836175,    0.467068187028852,   0.648198406466157,
    0.0252281814930363,   0.842206612419335,   0.559032544988695,
    0.854099949273443,    0.347879194327261,   0.446026648055103,
    0.0542394844411296,   0.177107533789109,   0.662808061960974,
    0.330828995203305,    0.898486137834300,   0.118155198446711,
    0.988417928784981,    0.539982099037929,   0.706917419322763,
    0.999491620097705,    0.287849344815137,   0.414522538893108,
    0.464839941625137,    0.763957078491957,   0.818204038907671,
    0.100221540195492,    0.178116953886766,   0.359634913482080,
    0.0567046890682912,   0.521885673661279,   0.335848974676925,
    0.175669029675661,    0.208946673993135,   0.905153559004464,
    0.675391177336247,    0.468468199903997,   0.912132474239623,
    0.104011574779379,    0.745546073701717,   0.736267455596639
};
static float32_t test_conv_input_f32_B[300] = {
    0.561861425281637,  0.184194097515527,  0.597211350337855,
    0.299936990089789,  0.134122932828682,  0.212601533358843,
    0.894941675440814,  0.0714528127870445, 0.242486558936719,
    0.0537543922087138, 0.441722057064424,  0.0132832004672525,
    0.897191350973572,  0.196658191367632,  0.0933705167550930,
    0.307366899587920,  0.456057666843742,  0.101669393624755,
    0.995389727655092,  0.332092833452499,  0.297346815887922,
    0.0620452213196326, 0.298243971887764,  0.0463512648981808,
    0.505428142457703,  0.761425886690113,  0.631069999213594,
    0.0898916507760917, 0.0808624231303137, 0.777240536548943,
    0.905134744223571,  0.533771951767000,  0.109154212042459,
    0.825808857855598,  0.338097718802172,  0.293973053026484,
    0.746313427703679,  0.0103366183433964, 0.0484473392532221,
    0.667916121573624,  0.603467983830770,  0.526102465795561,
    0.729709448223228,  0.707253485315422,  0.781377051799277,
    0.287976975614171,  0.692531986386519,  0.556669834964013,
    0.396520792581593,  0.0615906670539647, 0.780175531491174,
    0.337583864052045,  0.607865907262946,  0.741254049502218,
    0.104813241973500,  0.127888379782995,  0.549540107015198,
    0.485229408584959,  0.890475679184438,  0.798960278812879,
    0.734341083695970,  0.0513318861123711, 0.0728852990989761,
    0.0885274596747204, 0.798350864113952,  0.943008139570703,
    0.683715572408358,  0.132082955713563,  0.722724539656766,
    0.110353480642349,  0.117492852151833,  0.640717922965926,
    0.328814214756803,  0.653812022595774,  0.749131463103519,
    0.583185731454876,  0.740032327987778,  0.234826914747904,
    0.734957541696052,  0.970598525086615,  0.866930291751916,
    0.0862345298634963, 0.366436616319199,  0.369198804330018,
    0.685028472661609,  0.597941635383889,  0.789363943641905,
    0.367652918437877,  0.206027859505195,  0.0866665473955323,
    0.771933917099107,  0.205674521464760,  0.388271631047802,
    0.551778531957228,  0.228953252023100,  0.641940620399187,
    0.484480372398180,  0.151845525116267,  0.781931966588002,
    0.100606322362422,  0.294066333758628,  0.237373019705579,
    0.530872257027928,  0.0914987313394122, 0.405315419880591,
    0.104846247115757,  0.112283962156027,  0.784427890743913,
    0.291570317906931,  0.603533438750887,  0.964422667215901,
    0.432484993970361,  0.694752194617940,  0.758099275289454,
    0.432642326147101,  0.655498039803537,  0.109755050723052,
    0.933759848385332,  0.187460806421687,  0.266178838907639,
    0.797830260211597,  0.487603775433924,  0.768958264058869,
    0.396006745217875,  0.272938794123691,  0.0372346340703280,
    0.673294914108653,  0.429564459251853,  0.451739234904736,
    0.609857169290216,  0.0594032968582772, 0.315811438338866,
    0.772722130862935,  0.696432989006095,  0.125332181109180,
    0.130151450389424,  0.0923523387192016, 0.00782029356933489,
    0.423109385164167,  0.655573174937914,  0.722922524692024,
    0.531209293582439,  0.108817938273045,  0.631766373528489,
    0.126499865329303,  0.134303304313575,  0.0985940927109977,
    0.142027248431928,  0.168251298491528,  0.196248922256955,
    0.317479775149435,  0.316428999146291,  0.217563309422821,
    0.251041846015736,  0.892922405285977,  0.703223224556291,
    0.555737942719387,  0.184433667757653,  0.212030842532321,
    0.0773468081126768, 0.913800410779568,  0.706715217696931,
    0.557788966754876,  0.313428989936591,  0.166203562902151,
    0.622497259279895,  0.987934734952495,  0.170432023056883,
    0.257792250572013,  0.396799318633144,  0.0739947695769380,
    0.684096066962009,  0.402388332696162,  0.982835201393951,
    0.402183985222485,  0.620671947199578,  0.154369805479272,
    0.381345204444472,  0.161133971849361,  0.758112431327419,
    0.871111121915389,  0.350776744885893,  0.685535708747537,
    0.294148633767850,  0.530629303856886,  0.832423386285184,
    0.597490191872579,  0.335311330705246,  0.299225023333107,
    0.452592541569324,  0.422645653220462,  0.359606317972236,
    0.558319199869297,  0.742545365701939,  0.424334783625691,
    0.429355788576205,  0.124872758719813,  0.0244340160503740,
    0.290185265130727,  0.317520582899226,  0.653690133966475,
    0.956935924070684,  0.935730872784880,  0.457886333854367,
    0.240478396832085,  0.763897944286478,  0.759327383131096,
    0.740648064978614,  0.743688341487326,  0.105920416732765,
    0.681560430470316,  0.463260578593719,  0.212163205254934,
    0.0985187376881084, 0.823574473927839,  0.175009737382080,
    0.163569909784993,  0.665987216411111,  0.894389375354243,
    0.516558208351270,  0.702702306950475,  0.153590376619400,
    0.953457069886248,  0.540884081241477,  0.679733898210467,
    0.0365630180484529, 0.809203851293793,  0.748618871776197,
    0.120187017987081,  0.525045164762609,  0.325833628763249,
    0.546449439903069,  0.398880752383199,  0.415093386613047,
    0.180737760254794,  0.255386740488051,  0.0205357746581846,
    0.923675612620407,  0.653699889008253,  0.932613572048564,
    0.163512368527526,  0.921097255892198,  0.794657885388753,
    0.577394196706649,  0.440035595760254,  0.257613736712438,
    0.751946393867450,  0.228669482105501,  0.0641870873918986,
    0.767329510776574,  0.671202185356536,  0.715212514785840,
    0.642060828433852,  0.419048293624883,  0.390762082204175,
    0.816140102875323,  0.317427863655850,  0.814539772900651,
    0.789073514938958,  0.852263890343846,  0.505636617571756,
    0.635661388861377,  0.950894415378135,  0.443964155018810,
    0.0600188197794760, 0.866749896999319,  0.631188734269011,
    0.355073651878849,  0.997003271606648,  0.224171498983127,
    0.652451072968615,  0.604990641908259,  0.387245431483135,
    0.142187159290504,  0.0251349857102031, 0.421112253765241,
    0.184100289427511,  0.725775267469453,  0.370362686515198,
    0.841560087468206,  0.734229691193313,  0.571025872824379,
    0.176855057625303,  0.957384022595723,  0.265322036192920,
    0.924580895239601,  0.223770404697041,  0.373563807642645,
    0.0875003495765857, 0.640116548246715,  0.180616887753108,
    0.0450511074735742, 0.723173479183095,  0.347437645581790,
    0.660616824502904,  0.383868601071971,  0.627346502443467,
    0.0216498146303065, 0.910569988523029,  0.800558656278811
};
static q31_t test_conv_input_q31_A[300] = {
    17, 10, 26, 48, 20, 30, 31, 36, 21, 43, 47, 38, 15, 40, 8,  26, 31, 36, 20,
    37, 45, 3,  7,  44, 22, 21, 48, 38, 50, 12, 5,  20, 26, 29, 49, 25, 21, 50,
    14, 34, 49, 34, 15, 27, 1,  45, 21, 16, 48, 24, 15, 5,  30, 8,  4,  30, 15,
    19, 37, 43, 18, 49, 48, 11, 39, 31, 46, 31, 36, 38, 20, 13, 2,  24, 33, 14,
    26, 13, 15, 33, 45, 44, 11, 20, 45, 13, 49, 31, 9,  42, 33, 28, 13, 3,  12,
    19, 32, 50, 11, 38, 45, 24, 8,  41, 24, 6,  44, 32, 5,  46, 2,  2,  50, 35,
    19, 26, 39, 3,  37, 36, 23, 30, 17, 9,  20, 46, 12, 19, 17, 5,  26, 42, 46,
    37, 20, 15, 35, 45, 47, 5,  25, 7,  13, 45, 10, 7,  28, 16, 20, 40, 42, 35,
    21, 33, 11, 31, 34, 31, 18, 19, 9,  40, 25, 18, 39, 12, 43, 41, 43, 19, 20,
    44, 24, 29, 35, 49, 28, 32, 29, 47, 44, 9,  9,  13, 38, 10, 50, 36, 9,  43,
    46, 49, 29, 29, 9,  26, 28, 9,  25, 27, 10, 32, 2,  16, 27, 17, 31, 19, 7,
    46, 33, 33, 34, 38, 43, 26, 8,  20, 42, 9,  17, 49, 41, 12, 50, 4,  22, 21,
    21, 6,  22, 31, 50, 11, 18, 14, 15, 10, 2,  23, 13, 44, 27, 46, 49, 30, 6,
    47, 30, 45, 22, 31, 4,  47, 33, 6,  36, 20, 5,  11, 13, 12, 36, 38, 28, 28,
    32, 50, 32, 31, 46, 29, 17, 48, 22, 31, 37, 34, 11, 5,  14, 44, 28, 24, 22,
    39, 33, 33, 9,  22, 26, 19, 25, 18, 39, 20, 36, 25, 37, 47
};
static q31_t test_conv_input_q31_B[300] = {
    26, 46, 11, 44, 5,  24, 2,  41, 9,  9,  10, 35, 11, 15, 39, 26, 46, 3,  22,
    29, 29, 42, 7,  16, 1,  48, 39, 38, 7,  18, 8,  25, 41, 32, 35, 32, 37, 43,
    32, 10, 29, 9,  46, 4,  17, 30, 24, 41, 27, 12, 36, 8,  33, 32, 12, 10, 9,
    8,  11, 48, 1,  48, 2,  49, 15, 27, 44, 45, 10, 34, 48, 49, 6,  9,  38, 3,
    4,  25, 43, 50, 1,  28, 44, 46, 43, 44, 38, 6,  26, 9,  42, 47, 9,  45, 20,
    20, 14, 44, 38, 23, 36, 48, 9,  13, 33, 41, 43, 20, 6,  5,  19, 42, 11, 40,
    33, 2,  40, 47, 25, 42, 7,  38, 47, 42, 13, 11, 27, 20, 24, 50, 31, 48, 25,
    22, 39, 38, 23, 3,  5,  40, 33, 2,  28, 36, 6,  11, 41, 7,  45, 47, 1,  19,
    9,  28, 6,  2,  47, 49, 19, 33, 4,  11, 2,  24, 8,  50, 22, 48, 37, 30, 28,
    36, 1,  40, 47, 1,  42, 39, 50, 12, 46, 33, 6,  14, 39, 41, 6,  24, 11, 47,
    17, 43, 13, 44, 10, 38, 2,  33, 29, 19, 11, 40, 8,  25, 1,  10, 25, 42, 8,
    37, 35, 2,  25, 49, 6,  38, 32, 30, 25, 29, 22, 4,  15, 29, 32, 47, 49, 5,
    34, 31, 24, 18, 24, 34, 48, 5,  40, 30, 46, 6,  15, 3,  26, 39, 15, 12, 17,
    23, 37, 26, 20, 46, 49, 32, 7,  31, 20, 50, 15, 36, 27, 10, 35, 3,  10, 3,
    45, 42, 6,  21, 7,  29, 48, 13, 50, 18, 11, 34, 49, 32, 4,  19, 9,  12, 3,
    46, 40, 19, 42, 38, 32, 20, 18, 5,  18, 28, 24, 33, 26, 41
};
static q15_t test_conv_input_q15_A[100] = {
    2, 8, 3, 5, 2, 6, 3, 6, 7, 7, 5, 1, 3, 9, 2, 8, 5, 9, 1, 4, 1, 9, 1, 7, 8,
    8, 1, 4, 3, 8, 4, 9, 2, 3, 2, 2, 8, 6, 5, 2, 8, 6, 4, 5, 4, 1, 3, 2, 2, 3,
    4, 1, 9, 9, 5, 5, 4, 9, 4, 2, 8, 4, 3, 4, 1, 2, 9, 9, 6, 1, 3, 4, 8, 1, 1,
    2, 6, 7, 6, 5, 5, 3, 7, 2, 7, 2, 4, 6, 8, 1, 9, 7, 5, 4, 5, 3, 5, 5, 8, 8
};
static q15_t test_conv_input_q15_B[100] = {
    6, 4, 8, 5, 4, 9, 8, 5, 6, 6, 2, 3, 5, 3, 8, 2, 3, 2, 3, 4, 3, 9, 4, 2, 9,
    9, 4, 2, 3, 4, 6, 3, 6, 7, 2, 2, 3, 3, 4, 5, 1, 3, 8, 1, 9, 7, 5, 6, 3, 5,
    9, 5, 5, 3, 5, 6, 7, 4, 4, 9, 1, 8, 9, 8, 1, 3, 4, 7, 2, 7, 1, 6, 5, 8, 7,
    9, 9, 4, 7, 2, 1, 7, 5, 5, 9, 6, 6, 8, 8, 6, 2, 3, 8, 1, 5, 2, 9, 7, 5, 5
};
static q7_t test_conv_input_q7_A[5] = {2, 4, 1, 4, 7};
static q7_t test_conv_input_q7_B[5] = {1, 7, 3, 6, 2};
static float32_t output_f32[599] = {0}, output_f32_ref[599] = {0};
static q31_t output_q31[599] = {0}, output_q31_ref[599] = {0};
static q15_t output_q15[199] = {0}, output_q15_ref[199] = {0};
static q7_t output_q7[9] = {0}, output_q7_ref[9] = {0};
static q15_t pScratch1[298] = {0}, pScratch2[100] = {0};
static q15_t pScratch3[13] = {0}, pScratch4[5] = {0};
static int test_flag_error = 0;

#define DELTAF32 (0.05f)
#define DELTAQ31 (63)
#define DELTAQ15 (2)
#define DELTAQ7 (2)

int main(void)
{
    printf("\r\nNuclei RISC-V NMSIS-DSP Library Demonstration\r\n");
#if (defined(__RISCV_FEATURE_DSP) && __RISCV_FEATURE_DSP == 1) || \
    (defined(__RISCV_FEATURE_VECTOR) && __RISCV_FEATURE_VECTOR == 1)
    printf("Using Nuclei RISC-V accelerated and optimized NMSIS-DSP library!\r\n");
#if (defined(__RISCV_FEATURE_DSP) && __RISCV_FEATURE_DSP == 1)
    printf("Warning: Make sure Nuclei RISC-V DSP(P-ext) is present in your CPU core!\r\n");
#endif
#if (defined(__RISCV_FEATURE_VECTOR) && __RISCV_FEATURE_VECTOR == 1)
    printf("Warning: Make sure Nuclei RISC-V Vector is present in your CPU core!\r\n");
    // Enable Vector Unit
    __RV_CSR_SET(CSR_MSTATUS, 0x200);
#endif
    printf("         Otherwise this example will trap to cpu core exception!\r\n\r\n");
#else
    printf("Using c-only optimized NMSIS-DSP library!\r\n");
#endif

    BENCH_INIT();

    BENCH_START(riscv_conv_q31);
    riscv_conv_q31(test_conv_input_q31_A, 300, test_conv_input_q31_B, 300,
                   output_q31);
    BENCH_END(riscv_conv_q31);
    BENCH_START(ref_conv_q31);
    ref_conv_q31(test_conv_input_q31_A, 300, test_conv_input_q31_B, 300,
                 output_q31_ref);
    BENCH_END(ref_conv_q31);
    for (int i = 0; i < 599; i++) {
        if (labs(output_q31_ref[i] - output_q31[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_conv_q31);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q31_ref[i],
                   output_q31[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_q31);

    BENCH_START(riscv_conv_q15);
    riscv_conv_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B, 100,
                   output_q15);
    BENCH_END(riscv_conv_q15);
    BENCH_START(ref_conv_q15);
    ref_conv_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B, 100,
                 output_q15_ref);
    BENCH_END(ref_conv_q15);
    for (int i = 0; i < 199; i++) {
        if (abs(output_q15_ref[i] - output_q15[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_conv_q15);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q15_ref[i],
                   output_q15[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_q15);

    BENCH_START(riscv_conv_q7);
    riscv_conv_q7(test_conv_input_q7_A, 5, test_conv_input_q7_B, 5, output_q7);
    BENCH_END(riscv_conv_q7);
    BENCH_START(ref_conv_q7);
    ref_conv_q7(test_conv_input_q7_A, 5, test_conv_input_q7_B, 5,
                output_q7_ref);
    BENCH_END(ref_conv_q7);
    for (int i = 0; i < 9; i++) {
        if (abs(output_q7_ref[i] - output_q7[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_conv_q7);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q7_ref[i],
                   output_q7[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_q7);

    BENCH_START(riscv_conv_fast_q15);
    riscv_conv_fast_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B, 100,
                        output_q15);
    BENCH_END(riscv_conv_fast_q15);
    BENCH_START(ref_conv_fast_q15);
    ref_conv_fast_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B, 100,
                      output_q15_ref);
    BENCH_END(ref_conv_fast_q15);
    for (int i = 0; i < 199; i++) {
        if (abs(output_q15_ref[i] - output_q15[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_conv_fast_q15);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q15_ref[i],
                   output_q15[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_fast_q15);

    BENCH_START(riscv_conv_fast_q31);
    riscv_conv_fast_q31(test_conv_input_q31_A, 300, test_conv_input_q31_B, 300,
                        output_q31);
    BENCH_END(riscv_conv_fast_q31);
    BENCH_START(ref_conv_fast_q31);
    ref_conv_fast_q31(test_conv_input_q31_A, 300, test_conv_input_q31_B, 300,
                      output_q31_ref);
    BENCH_END(ref_conv_fast_q31);
    for (int i = 0; i < 599; i++) {
        if (labs(output_q31_ref[i] - output_q31[i]) > DELTAQ31) {
            BENCH_ERROR(riscv_conv_fast_q31);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q31_ref[i],
                   output_q31[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_fast_q31);

    BENCH_START(riscv_conv_opt_q15);
    riscv_conv_opt_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B, 100,
                       output_q15, pScratch1, pScratch2);
    BENCH_END(riscv_conv_opt_q15);
    BENCH_START(ref_conv_opt_q15);
    ref_conv_opt_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B, 100,
                     output_q15_ref, pScratch1, pScratch2);
    BENCH_END(ref_conv_opt_q15);
    for (int i = 0; i < 199; i++) {
        if (abs(output_q15_ref[i] - output_q15[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_conv_opt_q15);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q15_ref[i],
                   output_q15[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_opt_q15);

    BENCH_START(riscv_conv_opt_q7);
    riscv_conv_opt_q7(test_conv_input_q7_A, 5, test_conv_input_q7_B, 5,
                      output_q7, pScratch3, pScratch4);
    BENCH_END(riscv_conv_opt_q7);
    BENCH_START(ref_conv_opt_q7);
    ref_conv_opt_q7(test_conv_input_q7_A, 5, test_conv_input_q7_B, 5,
                    output_q7_ref, pScratch3, pScratch4);
    BENCH_END(ref_conv_opt_q7);
    for (int i = 0; i < 9; i++) {
        if (abs(output_q7_ref[i] - output_q7[i]) > DELTAQ7) {
            BENCH_ERROR(riscv_conv_opt_q7);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q7_ref[i],
                   output_q7[i]);
            test_flag_error = 1;
        }
    }

    BENCH_STATUS(riscv_conv_opt_q7);

    BENCH_START(riscv_conv_fast_opt_q15);
    riscv_conv_fast_opt_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B,
                            100, output_q15, pScratch1, pScratch2);
    BENCH_END(riscv_conv_fast_opt_q15);
    BENCH_START(ref_conv_fast_opt_q15);
    ref_conv_fast_opt_q15(test_conv_input_q15_A, 100, test_conv_input_q15_B,
                          100, output_q15_ref, pScratch1, pScratch2);
    BENCH_END(ref_conv_fast_opt_q15);
    for (int i = 0; i < 199; i++) {
        if (abs(output_q15_ref[i] - output_q15[i]) > DELTAQ15) {
            BENCH_ERROR(riscv_conv_fast_opt_q15);
            printf("index: %d, expect: %d, actual: %d\n", i, output_q15_ref[i],
                   output_q15[i]);
            test_flag_error = 1;
        }
    }
    BENCH_STATUS(riscv_conv_fast_opt_q15);
    if (test_flag_error) {
        printf("test error apprears, please recheck.\n");
        return 1;
    } else {
        printf("all test are passed. Well done!\n");
    }
    return 0;
}
