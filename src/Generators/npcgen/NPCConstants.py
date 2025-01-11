#!/c/Python22/python

"""
 *******************************************************************************
 * 
 * $RCSfile: NPCConstants.py,v $
 * $Date: 2009-08-28 12:31:32 -0500 (Fri, 28 Aug 2009) $
 *
 * Copyright (c) 2002,2003  Andrew C. Yinger
 * 443  50th Street, Oakland  CA
 * All rights reserved.
 *
 * Description: 
 *
 * NPC constant, or static, data.
 *
 *******************************************************************************
"""

# constant data
kNPCGenHTMLDocType        = '<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">'
kNPCGenCSSLink            = '<LINK REL=STYLESHEET HREF="%s/NPCGen.css" TYPE="text/css">'

kHighParrySkills        = frozenset(('fencing', 'main-gauche', 'staff', 'short-staff'))
kMainAttributeKeys        = frozenset(('stats', 'advantages', 'disadvantages'))
kMainStatKeys            = frozenset(('strength', 'dexterity', 'intelligence', 'health'))
kSkillAttributeKeys        = frozenset(('attributes', 'skills'))
kItemAttributeKeys        = frozenset(('weapons', 'armor'))
kOldHair                = frozenset(('gray', 'white'))
kBaldHair                = frozenset(('bald', 'brown-balding'))
kThreeColumnHeadings    = frozenset(('advantages/disadvantages/quirks', 'skills', 'stats/secondary stats', 'appearance'))
kNonSkillAttributeKeys    = frozenset(('advantages/disadvantages/quirks', 'stats/secondary stats'))
kNewLineModItemKeys        = frozenset(('*mod0', '*mod3', '*mod6'))
kZeroValueAttributes    = frozenset(('rank', 'paramilitary-rank', 'magery', 'eidetic-memory', 'pacifism', 'lame'))

kEnhancedParryModifiers    = ('combat-reflexes', 'enhanced-parry', 'passive-defense')
kEnhancedDodgeModifiers = ('combat-reflexes', 'enhanced-dodge', 'passive-defense')
kEnhancedBlockModifiers = ('combat-reflexes', 'enhanced-block', 'passive-defense')
kBaseDRModifiers        = ('toughness', 'damage-resistance')
kMainAttribKeyOrder        = ('stats', 'appearance', 'attributes', 'advantages', 'disadvantages', 'quirks', 'skills', 'maneuvers', 'weapons', 'armor', 'combat')
kDisplayAttribKeyOrder    = ('appearance', 'stats/secondary stats', 'advantages/disadvantages/quirks', 'skills', 'combat', 'weapons', 'armor')
kAppearanceKeyOrder        = ('sex', 'hair', 'eyes', 'age', 'height', 'weight', 'nationality', 'tech-level', 'genre')
kStatAttribKeyOrder        = ('strength', 'dexterity', 'intelligence', 'health', 'hit-points', 'fatigue', 'speed', 'move', 'will', 'vision', 'hearing', 'taste-smell')
kItemKeyOrder            = ('name', 'quantity', 'damage', 'db', 'dr', 'area', 'weight', 'rel', 'dam', 'ss', 'acc', 'rof', 'shots', 'st',
                           'rcl', 'rch', 'note', 'doses', 'ammo', '*mod0', '*mod1', '*mod2', '*mod3', '*mod4', '*mod5', '*mod6', '*mod7', '*mod8')
kUnarmedParrySkills        = frozenset(('brawling', 'boxing', 'martial-punch', 'martial-throw', 'karate', 'judo'))
kUnarmedSkillDivisorSets = (('martial-punch', 5.0), ('karate', 5.0), ('boxing', 5.0), ('brawling', 10.0))

kPhysicalSkillCosts        = (.5, 1, 2, 4, 8)
kMentalSkillCosts        = (.5, 1, 2, 4, 8)
kMentalVHSkillCosts        = (.5, 1, 2, 4, 8)
kBaseStatCosts            = (-10, 0, 10)


                    # str*: (level, desc)
kEncumbranceTable =  {
                        2:  (0,  'None'),
                        4:  (1,  'Light'),
                        6:  (2,  'Medium'),
                        12: (3,  'Heavy'),
                        20: (4,  'Extra-Heavy'),
                        30: (5,  'Super-Heavy'),
                        50: (10, 'Immobilized')
                    }

kStrengthTable = {
                        #     thrust,  swing
                    5    :    ((1, -5), (1, -5)),
                    6    :    ((1, -4), (1, -4)),
                    7    :    ((1, -3), (1, -3)),
                    8    :    ((1, -3), (1, -2)),
                    9    :    ((1, -2), (1, -1)),
                    10    :    ((1, -2), (1,  0)),
                    11    :    ((1, -1), (1,  1)),
                    12    :    ((1, -1), (1,  2)),
                    13    :    ((1,  0), (2, -1)),
                    14    :    ((1,  0), (2,  0)),
                    15    :    ((1,  1), (2,  1)),
                    16    :    ((1,  1), (2,  2)),
                    17    :    ((1,  2), (3, -1)),
                    18    :    ((1,  2), (3,  0)),
                    19    :    ((2, -1), (3,  1)),
                    20    :    ((2, -1), (3,  2)),
                    21    :    ((2,  0), (4, -1)),
                    22    :    ((2,  0), (4,  0)),
                    23    :    ((2,  1), (4,  1)),
                    24    :    ((2,  1), (4,  2)),
                    25    :    ((2,  2), (5, -1)),
                    26    :    ((2,  2), (5,  0)),
                    27    :    ((3, -1), (5,  1)),
                    28    :    ((3, -1), (5,  2)),
                    29    :    ((3,  0), (6, -1)),
                    30    :    ((3,  0), (6,  0))
                }

kStrengthHeightMap = {
                        #st :  height"
                        5    :  64,
                        6    :  65,
                        7    :  66,
                        8    :  67,
                        9    :  68,
                        10    :  69,
                        11    :  70,
                        12    :  71,
                        13    :  72,
                        14    :  73,
                        15    :  74,
                        16    :  75,
                        17    :  76,
                        18    :  77,
                        19    :  78,
                        20    :  79,
                        21    :  80,
                        22    :  81,
                        23    :  83,
                        24    :  85,
                        25    :  86,
                        26    :  87,
                    }

kHeightWeightMap = {
                        #ht":  lbs
                        30    :  40,
                        31    :  42,
                        32    :  44,
                        33    :  45,
                        34    :  46,
                        35    :  47,
                        36    :  48,
                        37    :  49,
                        38    :  50,
                        39    :  52,
                        40    :  54,
                        41    :  56,
                        42    :  58,
                        43    :  60,
                        44    :  62,
                        45    :  64,
                        46    :  66,
                        47    :  68,
                        48    :  70,
                        49    :  72,
                        50    :  74,
                        51    :  76,
                        52    :  78,
                        53    :  81,
                        54    :  84,
                        55    :  87,
                        56    :  90,
                        57    :  95,
                        58    :  100,
                        59    :  105,
                        60    :  110,
                        61    :  115,
                        62    :  120,
                        63    :  125,
                        64    :  130,
                        65    :  132,
                        66    :  135,
                        67    :  140,
                        68    :  145,
                        69    :  150,
                        70    :  155,
                        71    :  160,
                        72    :  165,
                        73    :  170,
                        74    :  180,
                        75    :  190,
                        76    :  200,
                        77    :  210,
                        78    :  220,
                        79    :  230,
                        80    :  240,
                        81    :  250,
                        82    :  260,
                        83    :  270,
                        84    :  280,
                        85    :  290,
                        86    :  300,
                        87    :  310,
                        88    :  320,
                        89    :  330,
                        90    :  340,
                        91    :  350,
                        92    :  360,
                        93    :  370,
                        94    :  380,
                        95    :  390,
                        96    :  400,
                        97    :  410,
                        98    :  420,
                        99    :  430,
                        100    :  440,
                        101 :  450,
                        102 :  460,
                        103 :  470,
                        104 :  480,
                        105 :  490,
                        106 :  495,
                        107 :  500,
                        108 :  505,
                        109 :  510,
                        110 :  515,
                    }

kEyeColours = (
                'brown',
                'brown',
                'dark-brown',
                'dark-brown',
                'dark-brown',
                'light-brown',
                'light-brown',
                'hazel',
                'blue-gray',
                'blue',
                'green',
                'brown'
              )

kHairColours = (
                'brown',
                'auburn',
                'red-brown',
                'dark-brown',
                'light-brown',
                'light-brown',
                'brown',
                'brown',
                'brown',
                'blonde',
                'dirty-blonde',
                'light-blonde',
                'strawberry-blonde',
                'white-blonde',
                'red',
                'red-brown',
                'orange-red',
                'gray',
                'white',
                'black',
                'black',
                'black',
                'jet-black',
                'blue-black',
                'brown-balding',
                'bald',
                'bald'
              )

kRankLevels = {
                0:    ('Private', 'Private', 'Private 1st Class'),
                1:    ('Corporal', 'Corporal', 'Corporal', 'Sergeant', 'Sergeant', 'Staff Sergeant'),
                2:    ('Sergeant 1st Class', 'Sergeant 1st Class', 'Sergeant 1st Class', 'Gunnery Sergeant', 'Master Sergeant', 'Master Sergeant', 'First Sergeant', 'Sergeant Major'),
                3:    ('2nd Lieutenant', '2nd Lieutenant', '1st Lieutenant'),
                4:    ('Captain', 'Captain', 'Major'),
                5:    ('Lt. Colonel',),
                6:    ('Colonel',),
                7:    ('Brigadier General', 'Major General'),
                8:    ('Lt. General', 'Lt. General', 'Lt. General', 'General', 'General', 'General of the Army')
              }

kParaRankLevels = {
                0:    ('Cadet', 'Officer', 'Officer'),
                1:    ('Senior Officer', 'Sergeant', 'Sergeant'),
                2:    ('Senior Segeant', 'Detective Sergeant',),
                3:    ('Lieutenant', 'Detective Lieutenant'),
                4:    ('Captain', 'Captain', 'Commander'),
                5:    ('Deputy Chief',),
                6:    ('Chief of Police',)
              } 

"""
 *******************************************************************************
 * $Log: NPCConstants.py,v $
 * Revision 2.7  2004/03/13 18:08:46  andrew
 * str damage, up to str 30
 *
 * Revision 2.6  2004/01/12 02:33:16  andrew
 * refactor to support new iewin version
 *
 * Revision 2.5  2004/01/10 15:30:59  andrew
 * changes to support point calculation
 *
 * Revision 2.4  2003/10/09 00:36:59  andrew
 * create static cache of commonly used sets
 *
 * Revision 2.3  2003/10/08 18:41:54  andrew
 * use new set types for sequences that need hash() function
 *
 * Revision 2.2  2003/07/12 00:31:56  ayinger
 * fail to force rev, weird
 *
 * Revision 2.1  2003/07/11 20:51:28  ayinger
 * add RCS tags
 *
 *******************************************************************************
"""
