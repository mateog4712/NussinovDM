#include <vector>
#include <string>
#include "testCases.hpp"
using namespace std;

string s1 = "GGAUACGGCCAUACUGCGCAGAAAGCAC";
string s2 = "GGAUACGGCCAUACUGCGCAGAAAGCACCGCUUCCCAUCCGAACAGCG";
string s3 = "GGAUACGGCCAUACUGCGCAGAAAGCACCGCUUCCCAUCCGAACAGCGAAGUUAAGCUGCGCCAGGCGGUGUUAGUACUGGGGUGGGCGACCACCCGGGAAUCCACCGUGCCGUAUCCU";
string s4 = "GGACCUGGUGGCUAUGGCGGGAGAGAUCCACCCGAUCCCAUCCCGAACUCGGCCGUGAAAACCCCCAGCGCCUAUGAUACUGCGGCUUAAGCCGUGGGAAAGUCGGUCGCCGCCAGGUCC";
string s5 = "GCCAACGUCCAUACCACGUUGAAAGCACCGGUUCUCGUCCGAUCACCGAAGUUAAGCAGCGUCGGGCGCGGUUAGUACUUGGAUGGGUGACCGCCUGGGAACCCCGCGUGACGUUGGCAU";
string s6 = "UAGGUUUGGUCCUAGCCUUUCUAUUAACUCUUAGUAGGAUUACACAUGCAAGCAUCCCCGCCCCAGUGAGUCACCCUCUAAAUCACCACGAUCAAAAGGAACAAGCAUCAAGUACGCAGAAAUGCAGCUCAAAACGCUUAGCCUAGCCACACCCCCACGGGAGACAGCAGUGAUAAACCUUUAGCAAUAAACGAAAGUUUAACUAAGCCAUACUAACCCCAGGGUUGGUCAAUUUCGUGCCAGCCACCGCGGUCACACGAUUAACCCAAGCCAAUAGAAAUCGGCGUAAAGAGUGUUUUAGAUCAAUCCCCCAAUAAAGCUAAAAUUCACCUGAGUUGUAAAAAACUCCAGCUGAUAUAAAAUAAACUACGAAAGUGGCUUUAAUAUAUCUGAACACACAAUAGCUAGGACCCAAACUGGGAUUAGAUACCCCACUAUGCCUAGCCCUAAACUUCAACAGUUAAAUUAACAAGACUGCUCGCCAGAACACUACGAGCCACAGCUUAAAACUCAAAGGACCUGGCGGUGCUUCACAUCCUUCUAGAGGAGCCUGUUCUGUAAUCGAUAAACCCCGAUCAACCUCACCACCUCUUGCUCAGCCUAUAUACCGCCAUCUUCAGCAAACCCUGACGAAGGCCACAAAGUAAGCACAAGUACCCACGUAAAGACGUUAGGUCAAGGUGUAGCCCAUGAGGUGGCAAGAAAUGGGCUACAUUUUCUACUUCAGAAAACUACGAUAACCCUUAUGAAACCUAAGGGUAGAAGGUGGAUUUAGCAGUAAACUAAGAGUAGAGUGCUUAGUUGAACAGGGCCCUGAAGCGCGUACACACCGCCCGUCACCCUCCUCAAGUAUACUUCAAAGGACAUUUAACUAAAACCCCUACGCAUCUAUAUAGAGGAGAUAAGUCGUAACAUGGUAAGUGUACUGGAAAGUGCACUUGGACGAAC";
string s7 = "UUUUCUGAGAAUUUGAUCUUGGUUCAGAUUGAACGCUGGCGGCGUGGAUGAGGCAUGCAAGUCGAACGGAAUAAUGACUUCGGUUGUUAUUUAGUGGCGGAAGGGUUAGUAAUACAUAGAUAAUCUGUCCUCAACUUGGGAAUAACGGUUGGAAACGACCGCUAAUACCGAAUGUGGUAUGUUUAGGCAUCUAAAACAUAUUAAAGAAGGGGAUCUUCGGACCUUUCGGUUGAGGGAGAGUCUAUGGGAUAUCAGCUUGUUGGUGGGGUAAUGGCCUACCAAGGCUUUGACGUCUAGGCGGAUUGAGAGAUUGACCGCCAACACUGGGACUGAGACACUGCCCAGACUUCUACGGAAGGCUGCAGUCGAGAAUCUUUCGCAAUGGACGAAAGUCUGACGAAGCGACGCCGCGUGUGUGAUGAAGGCUCUAGGGUUGUAAAGCACUUUCGCUUGGGAAUAAGAGAGAUUGGCUAAUAUCCAAUCGAUUUGAGCGUACCAGGUAAAGAAGCACCGGCUAACUCCGUGCCAGCAGCUGCGGUAAUACGGAGGGUGCUAGCGUUAAUCGGAUUUAUUGGGCGUAAAGGGCGUGUAGGCGGAAAGGAAAGUUAGAUGUUAAAUCUUGGGGCUCAACCCCAAGCCAGCAUCUAAUACUAUCUUUCUAGAGGGUAGAUGGAGAAAAGGGAAUUCCACGUGUAGCGGUGAAAUGCGUAGAUAUGUGGAAGAACACCAGUGGCGAAGGCGCUUUUCUAAUUUACACCUGACGCUAAGGCGCGAAAGCAAGGGGAGCAAACAGGAUUAGAUACCCUGGUAGUCCUUGCCGUAAACGAUGCAUACUUGAUGUGGAUAGUCUCAACCCUAUCCGUGUCGUAGCUAACGCGUUAAGUAUGCCGCCUGAGGAGUACACUCGCAAGGGUGAAACUCAAAAGAAUUGACGGGGGCCCGCACAAGCAGUGGAGCAUGUGGUUUAAUUCGAUGCAACGCGAAGAACCUUACCUGGGCUUGACAUGUAUUUGACCGCGGCAGAAAUGUCGUUUUCCGCAAGGACAGAUACACAGGUGCUGCAUGGCUGUCGUCAGCUCGUGCCGUGAGGUGUUGGGUUAAGUCCCGCAACGAGCGCAACCCUUAUCGUUAGUUGCCAACACUUAGGGUGGGAACUCUAACGAGACUGCCUGGGUUAACCAGGAGGAAGGCGAGGAUGACGUCAAGUCAGCAUGGCCCUUAUGCCCAGGGCUACACACGUGCUACAAUGGCCAGUACAGAAGGUAGCAAUAUCGUGAGAUGGAGCAAAUCCUCAAAGCUGGCCCCAGUUCGGAUUGUAGUCUGCAACUCGACUACAUGAAGUCGGAAUUGCUAGUAAUGGCGUGUCAGCUAUAACGCCGUGAAUACGUUCCCGGGCCUUGUACACACCGCCCGUCACAUCAUGGGAGUUGGUUUUGCCUUAAGUCGUUGACUCAACCUGCAAAGGAGAGAGGCGCCCAAGGUGAGGCUGAUGACUGGGAUGAAGUCGUAACAAGGUAGCCCUACCGGAAGGUGGGGCUGGAUCACCUCCUUU";
string s8 = "AUCCAUGGAGAGUUUGAUCCUGGCUCAGGACGAACGCUGGCGGCAUGCUUAACACAUGCAAGUCGAACGAGCAAAGCAAUUUGUGUAGUGGCGAACGGGUGCGUAACGCGUAAGAACCUACCUAUCGGAGGGGGAUAACAUUGGGAAACUGUUGCUAAUACCCCAUACAGCUGAGGAGUGAAAGGUGAAAAACCGCCGAUAGAGGGGCUUGCGUCUGAUUAGCUAGUUGGUGGGGGUAACGGCCUCCCAAGGCCACGAGCAGUAGCUGGUCUGAGAGGAUGAUCAGCCACACUGGGACUGAGACACGGCCCAGACUCCUACGGGAGGCAGCAGUGAGGAAUUUUUCGCAAUGGGCGCAAGCGACGGAGCAAUGCCGCGUGCAGGAAGAAGGCCUGUGGGUCGUAAACUGCUUUUCUCAGAGAAGAAGUUCUGACGGUAUCUGAGGAAUAAGCACCGGCUAACUCUGUGCCAGCAGCCGCGGUAAUACAGAGGGUGCAAGCGUUGUCCGGAAUGAUUGGGCGUAAAGCGUCUGUAGGUGGCUCGUAAAGUCUAAUGUCAAAUACCAGGGCUCAACCUUGGACCGGCAUUGGAGUACUCACGAGCUUGAGUACGGUAGGGGCAGAGGGAAUUCCAUGUGGAGCGGUGAAAUGCGUAGAGAUAUGGAGGAACACCAGUGGCGAAGGCGCUCUGCUGGGCCGAAACUGACACUGAGAGACGAAAGCUGGGGGAGCGAAUAGGAUUAGAUACCCUAGUAGUCCCAGCCGUAAACUAUGGAGACUAAGUGCUGCCGCAAGCAGUGCUGUAGCUAACGCGUUAAGUCUCCCGCCUGGGGAGUAUGCUCGCAAGAGUGAAACUCAAAGGAAUUGACGGGACCGCACAAGCGGUGGAUUAUGUGGAUUAAUUCGAUACAACGCGAAGAACCUUACCAGGGUUUGACAUGUCAAGAACCUCUCAGAAAUGGGAGGGUGCCCUAACGGACUUGAACACAGGUGGUGCAUGGCUGUCGUCAGCUCGUGCUGUGAAGUGUAUAGUUAAGUCUCAUAACGAGCGCAACCCUCGUCUUUAGUUGCCAUUUGGUUCUCUAAAGAGACUGCCAGUGUAAGCUGGAGGAAGGUGAGGAUGACGUCAAGUCAGCAUGCCCCUUACAUCCUGGGCUUCACACGUAAUACAAUGGUUGGGACAAUCAGAAGCGACUCGUGAGAGCUAGCGGCUCUGUUAAACCCAACCUCAGUUCGGAUUGUAGGCUGCAACUCGCCUACAUGAAGCCGGAAUCGCUAGUAAUCGCCAGUCAGCUAUAUGGCGGUGAAUACGUUCCCGGGUCUUGUACACACCGCCCGUCACACCAUGGAAGCUGGUUCUGCUCCAAGUCGUUACCCUAACCUUCGGGAGGGGGGCGCCUAAAGCAGGGCUAGUGACUAGGGUGAAGUCGUAACAAGGUAGGGCUACUGGAAGGUGGCCCUGGCUCACCUCCUUC";
string s9 = "AAAUUGAAGAGUUUGAUCAUGGCUCAGAUUGAACGCUGGCGGCAGGCCUAACACAUGCAAGUCGAACGGUAACAGGAAGAAGCUUGCUUCUUUGCUGACGAGUGGCGGACGGGUGAGUAAUGUCUGGGAAACUGCCUGAUGGAGGGGGAUAACUACUGGAAACGGUAGCUAAUACCGCAUAACGUCGCAAGACCAAAGAGGGGGACCUUCGGGCCUCUUGCCAUCGGAUGUGCCCAGAUGGGAUUAGCUAGUAGGUGGGGUAACGGCUCACCUAGGCGACGAUCCCUAGCUGGUCUGAGAGGAUGACCAGCCACACUGGAACUGAGACACGGUCCAGACUCCUACGGGAGGCAGCAGUGGGGAAUAUUGCACAAUGGGCGCAAGCCUGAUGCAGCCAUGCCGCGUGUAUGAAGAAGGCCUUCGGGUUGUAAAGUACUUUCAGCGGGGAGGAAGGGAGUAAAGUUAAUACCUUUGCUCAUUGACGUUACCCGCAGAAGAAGCACCGGCUAACUCCGUGCCAGCAGCCGCGGUAAUACGGAGGGUGCAAGCGUUAAUCGGAAUUACUGGGCGUAAAGCGCACGCAGGCGGUUUGUUAAGUCAGAUGUGAAAUCCCCGGGCUCAACCUGGGAACUGCAUCUGAUACUGGCAAGCUUGAGUCUCGUAGAGGGGGGUAGAAUUCCAGGUGUAGCGGUGAAAUGCGUAGAGAUCUGGAGGAAUACCGGUGGCGAAGGCGGCCCCCUGGACGAAGACUGACGCUCAGGUGCGAAAGCGUGGGGAGCAAACAGGAUUAGAUACCCUGGUAGUCCACGCCGUAAACGAUGUCGACUUGGAGGUUGUGCCCUUGAGGCGUGGCUUCCGGAGCUAACGCGUUAAGUCGACCGCCUGGGGAGUACGGCCGCAAGGUUAAAACUCAAAUGAAUUGACGGGGGCCCGCACAAGCGGUGGAGCAUGUGGUUUAAUUCGAUGCAACGCGAAGAACCUUACCUGGUCUUGACAUCCACGGAAGUUUUCAGAGAUGAGAAUGUGCCUUCGGGAACCGUGAGACAGGUGCUGCAUGGCUGUCGUCAGCUCGUGUUGUGAAAUGUUGGGUUAAGUCCCGCAACGAGCGCAACCCUUAUCCUUUGUUGCCAGCGGUCCGGCCGGGAACUCAAAGGAGACUGCCAGUGAUAAACUGGAGGAAGGUGGGGAUGACGUCAAGUCAUCAUGGCCCUUACGACCAGGGCUACACACGUGCUACAAUGGCGCAUACAAAGAGAAGCGACCUCGCGAGAGCAAGCGGACCUCAUAAAGUGCGUCGUAGUCCGGAUUGGAGUCUGCAACUCGACUCCAUGAAGUCGGAAUCGCUAGUAAUCGUGGAUCAGAAUGCCACGGUGAAUACGUUCCCGGGCCUUGUACACACCGCCCGUCACACCAUGGGAGUGGGUUGCAAAAGAAGUAGGUAGCUUAACCUUCGGGAGGGCGCUUACCACUUUGUGAUUCAUGACUGGGGUGAAGUCGUAACAAGGUAACCGUAGGGGAACCUGCGGUUGGAUCACCUCCUUA";
string s10 = "CAUCCGGUCGAUCCUGCCGGAGCGCGACGCUCUCCCCAAGGACGAAGCCAUGCAUGCCCGCUCACCCGGGACGCGGCGGACGGCUCAGGACAACGGUUGCACCCCCCGCGGCGGUCCCUGCUAGCCGGACACCGCUGGCAACCCGGCGCCAAGACGUGCGCGCAAGGGCGGGCGCCCGCGGGCGAGCAGCGUGACGCAGCGACGGCCCGCCCGGGCUUCCGGGGCAUCACCCGGUCGGCGCGGUCGCGGCGCGCCGAGGGCCCGACGCCUGGCGGAGAAUCAGGGUUCGACUCCGGAGAGCGGGCCUGCGAGACGGCCCGCACAUCCAAGGACGGCAGCAGGCGCGGAACUUGCCCAAUGCGCGGCGCGCGAGGCAGCGACGGGGAGCGCGCGAGCGAGGCGGGCCCACAGCCCCCGCCGCGGAGCCGAGGGCAAGGUCUGGUGCCAGCAGCCGCGGUAAUUCCAGCUCGGCGAGCGUCGCGCGGCGCUGCUGCAGUUGAAACGCCCGUAGUUGGCCCCCCGCCGCCACGAGGAAACGGGAGCGCUCCAGGCAGGCCCGUUGGACCCGCCGCGUGGGACCGCGCAGCGGGCCGGCGCGCCGCGGCAGCCCCGAGGAGAGCGGGCGGGGGCACCGGUACCGGCCGGGGACGGGUGAAACAGGAUGAUCCCGCCGAGACCGCCGGCCGCGCAGGCGCCUGCCAAGACCGCCUCUGUCAAUCAAGGGCGAAGGCCGGGGGCUAGAAGGCGAUCAGACACCACCGUAUUCCCGGCCGUAAACGGUGCCGCCCCGCGGCCGGCGCGCGCGUCCCGCCGGCCGCCCAGGGAAACCGGGAGGCUCCGGGCUCUGGGGGGAGUAUGGCCGCAAGGCUGAAACUUGAAGGCAUUGACGGAGGGGUACCACCAGACGUGGAGUCUGCGGCUCAAUCUGACUCAACGCGCGCACCUCACCAGGCCCGGACGCGCGGAGGACCGACAGCCGGGCGCGCUUUCGCGAUCGCGCGGGCGGUGGUGCAUGGCCGCUCCCAGCCCGUGGCGCGAGCCGUCUGCUCCAUUGCGACAACGAGCGAGACCCCGGCCGCGGGCGCCGCGGGACGGCCCGCGCGAGCGGGAGGACGGCGGGGCGAUAGCAGGUCUGUGAUGCCCUCAGACGCCCUGGGCCGCACGCGCGCUACACUGGCGGGGCCAGCCGGCGCGCGCGAGGACGCGCGGAGCCCCCGCCGUGGCCGGGACCGCGGGCUGGAACGCCCCCGCGCACCAGGAAUGUCUUGUAGGCGCCCGCCCCCACCGCGCGCCGGACGCGUCCCUGCCCCUUGUACACACCGCCCGUCGCUCCUACCGACUGGGCGCGGCGGCGAGCGCCCCGGACGCGCGAAGGGCCGCGAGCCCCCGCGCCUGGAGGAAGGAGAAGUCGUAACAAGGUAUUCCGUAGGUGAACCUGCGGAUGGAUCCCUC";
//string s11 = "GGUUAAGCGACUAAGCGUACACGGUGGAUGCCCUGGCAGUCAGAGGCGAUGAAGGACGUGCUAAUCUGCGAUAAGCGUCGGUAAGGUGAUAUGAACCGUUAUAACCGGCGAUUUCCGAAUGGGGAAACCCAGUGUGUUUCGACACACUAUCAUUAACUGAAUCCAUAGGUUAAUGAGGCGAACCGGGGGAACUGAAACAUCUAAGUACCCCGAGGAAAAGAAAUCAACCGAGAUUCCCCCAGUAGCGGCGAGCGAACGGGGAGCAGCCCAGAGCCUGAAUCAGUGUGUGUGUUAGUGGAAGCGUCUGGAAAGGCGCGCGAUACAGGGUGACAGCCCCGUACACAAAAAUGCACAUGCUGUGAGCUCGAUGAGUAGGGCGGGACACGUGGUAUCCUGUCUGAAUAUGGGGGGACCAUCCUCCAAGGCUAAAUACUCCUGACUGACCGAUAGUGAACCAGUACCGUGAGGGAAAGGCGAAAAGAACCCCGGCGAGGGGAGUGAAAAAGAACCUGAAACCGUGUACGUACAAGCAGUGGGAGCACGCUUAGGCGUGUGACUGCGUACCUUUUGUAUAAUGGGUCAGCGACUUAUAUUCUGUAGCAAGGUUAACCGAAUAGGGGAGCCGAAGGGAAACCGAGUCUUAACUGGGCGUUAAGUUGCAGGGUAUAGACCCGAAACCCGGUGAUCUAGCCAUGGGCAGGUUGAAGGUUGGGUAACACUAACUGGAGGACCGAACCGACUAAUGUUGAAAAAUUAGCGGAUGACUUGUGGCUGGGGGUGAAAGGCCAAUCAAACCGGGAGAUAGCUGGUUCUCCCCGAAAGCUAUUUAGGUAGCGCCUCGUGAAUUCAUCUCCGGGGGUAGAGCACUGUUUCGGCAAGGGGGUCAUCCCGACUUACCAACCCGAUGCAAACUGCGAAUACCGGAGAAUGUUAUCACGGGAGACACACGGCGGGUGCUAACGUCCGUCGUGAAGAGGGAAACAACCCAGACCGCCAGCUAAGGUCCCAAAGUCAUGGUUAAGUGGGAAACGAUGUGGGAAGGCCCAGACAGCCAGGAUGUUGGCUUAGAAGCAGCCAUCAUUUAAAGAAAGCGUAAUAGCUCACUGGUCGAGUCGGCCUGCGCGGAAGAUGUAACGGGGCUAAACCAUGCACCGAAGCUGCGGCAGCGACGCUUAUGCGUUGUUGGGUAGGGGAGCGUUCUGUAAGCCUGCGAAGGUGUGCUGUGAGGCAUGCUGGAGGUAUCAGAAGUGCGAAUGCUGACAUAAGUAACGAUAAAGCGGGUGAAAAGCCCGCUCGCCGGAAGACCAAGGGUUCCUGUCCAACGUUAAUCGGGGCAGGGUGAGUCGACCCCUAAGGCGAGGCCGAAAGGCGUAGUCGAUGGGAAACAGGUUAAUAUUCCUGUACUUGGUGUUACUGCGAAGGGGGGACGGAGAAGGCUAUGUUGGCCGGGCGACGGUUGUCCCGGUUUAAGCGUGUAGGCUGGUUUUCCAGGCAAAUCCGGAAAAUCAAGGCUGAGGCGUGAUGACGAGGCACUACGGUGCUGAAGCAACAAAUGCCCUGCUUCCAGGAAAAGCCUCUAAGCAUCAGGUAACAUCAAAUCGUACCCCAAACCGACACAGGUGGUCAGGUAGAGAAUACCAAGGCGCUUGAGAGAACUCGGGUGAAGGAACUAGGCAAAAUGGUGCCGUAACUUCGGGAGAAGGCACGCUGAUAUGUAGGUGAGGUCCCUCGCGGAUGGAGCUGAAAUCAGUCGAAGAUACCAGCUGGCUGCAACUGUUUAUUAAAAACACAGCACUGUGCAAACACGAAAGUGGACGUAUACGGUGUGACGCCUGCCCGGUGCCGGAAGGUUAAUUGAUGGGGUUAGCGCAAGCGAAGCUCUUGAUCGAAGCCCCGGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGGUAAGUUCCGACCUGCACGAAUGGCGUAAUGAUGGCCAGGCUGUCUCCACCCGAGACUCAGUGAAAUUGAACUCGCUGUGAAGAUGCAGUGUACCCGCGGCAAGACGGAAAGACCCCGUGAACCUUUACUAUAGCUUGACACUGAACAUUGAGCCUUGAUGUGUAGGAUAGGUGGGAGGCUUUGAAGUGUGGACGCCAGUCUGCAUGGAGCCGACCUUGAAAUACCACCCUUUAAUGUUUGAUGUUCUAACGUUGACCCGUAAUCCGGGUUGCGGACAGUGUCUGGUGGGUAGUUUGACUGGGGCGGUCUCCUCCUAAAGAGUAACGGAGGAGCACGAAGGUUGGCUAAUCCUGGUCGGACAUCAGGAGGUUAGUGCAAUGGCAUAAGCCAGCUUGACUGCGAGCGUGACGGCGCGAGCAGGUGCGAAAGCAGGUCAUAGUGAUCCGGUGGUUCUGAAUGGAAGGGCCAUCGCUCAACGGAUAAAAGGUACUCCGGGGAUAACAGGCUGAUACCGCCCAAGAGUUCAUAUCGACGGCGGUGUUUGGCACCUCGAUGUCGGCUCAUCACAUCCUGGGGCUGAAGUAGGUCCCAAGGGUAUGGCUGUUCGCCAUUUAAAGUGGUACGCGAGCUGGGUUUAGAACGUCGUGAGACAGUUCGGUCCCUAUCUGCCGUGGGCGCUGGAGAACUGAGGGGGGCUGCUCCUAGUACGAGAGGACCGGAGUGGACGCAUCACUGGUGUUCGGGUUGUCAUGCCAAUGGCACUGCCCGGUAGCUAAAUGCGGAAGAGAUAAGUGCUGAAAGCAUCUAAGCACGAAACUUGCCCCGAGAUGAGUUCUCCCUGACCCUUUAAGGGUCCUGAAGGAACGUUGAAGACGACGACGUUGAUAGGCCGGGUGUGUAAGCGCAGCGAUGCGUUGAGCUAACCGGUACUAAUGAACCGUGAGGCUUAACCUU";
string s12 = "UAAGGCAGUGGUAGCGCUGAAGAAUGUUCGUGCAAUUGUCGUUAUUCAUUAUAAAAGGGCGGGUUUUAAAGGAUAUUUUAAAAUUUAAAACAAGCUUUUAAGAGCAGAUGGCGGAUGCCUUGCCAAAGAGAGGCGAUGAAGGACGUACUAGACUGCGAUAAGCUAUGCGGAGCUGUCAAGGAGCUUUGAUGCGUAGAUGUCCGAAUGGGGCAACCCAACUAAUAGAGAUAUUAGUUACUCUAACAGAGAGCGAACCUAGUGAAGUGAAACAUCUCAGUAACUAGAGGAAAAGAAAUCAACGAGAUUCCCUAAGUAGUGGCGAGCGAACGGGGAAAAGGGCAAACCGAGUGCUUGCAUUCGGGGUUGAGGACUGCAACAUCCAAGAGAACGCUUUAGCAGAGUUACCUGGAAAGGUAAGCCAUAGAAAGUGAUAGCCUUGUAUGCGACAAGGCGUUUUUAGGUAGCAGUAUCCAGAGUAGGCCAGGACACGAGGAAUCCAGGUUGAAGCCGGGGAGACCACUCUCCAACUCUAAAUACUACUCUUUGAGCGAUAGCGAACAAGUACCGUGAGGGAAAGGUGAAAAGAACCGCAGUGAGCGGAGUGAAAUAGAACCUGAAACCAUCUGCUUACAAUCAUUCAGAGCCCUAUGAUUUAUCAGGGUGAUGGACUGCCUUUUGCAUAAUGAUCCUGCGAGUUGUGGUAUCUGGCAAGGUUAAGCGAAUGCGAAGCCGUAGCGAAACGAGUUCUUAAUAGGGCGAACAAGUCAGAUGCUGCAGACCCGAAGCUAAGUGAUCUAUCCAUGGCCAAGUUGAAACGCGUGUAAUAGCGCGUGGAGGACUGAACUCCUACCCAUUGAAACGGGUUGGGAUGAGCUGUGGAUAGGGGUGAAAGGCCAAACAAACUUAGUGAUAGCUGGUUCUCUUCGAAAUAUAUUUAGGUAUAGCCUCAAGUGAUAAUAAAAGGGGGUAGAGCGCUGAUUGGGCUAGGGCUGCUCGCCGCGGUACCAAACCCUAUCAAACUUCGAAUACCUUUUAUCGUAUCUUGGGAGUCAGGCGGUGGGUGAUAAAAUCAAUCGUCAAAAGGGGAACAACCCAGACUACCAAAUAAGGUCCCUAAGUUCUAUUCUGAGUGGAAAAAGAUGUGUGGCUACUAAAACAACCAGGAGGUUGGCUUAGAAGCAGCCAUCCUUUAAAGAAAGCGUAACAGCUCACUGGUCUAGUGGUCAUGCGCUGAAAAUAUAACGGGGCUAAGAUAGACACCGAAUUUGUAGAUUGUGUUAAACACAGUGGUAGAAGAGCGUUCAUACCAGCGUUGAAGGUAUACCGGUAAGGAGUGCUGGAGCGGUAUGAAGUGAGCAUGCAGGAAUGAGUAACGAUAAGAUAUAUGAGAAUUGUAUCCGCCGUAAAUCUAAGGUUUCCUACGCGAUGGUCGUCAUCGUAGGGUUAGUCGGGUCCUAAGCCGAGUCCGAAAGGGGUAGGUGAUGGCAAAUUGGUUAAUAUUCCAAUACCGACUGUGGAGCGUGAUGGGGGGACGCAUAGGGUUAAGCGAGCUAGCUGAUGGAAGCGCUAGUCUAAGGGCGUAGAUUGGAGGGAAGGCAAAUCCACCUCUGUAUUUGAAACCCAAACAGGCUCUUUGAGUCCUUUUAGGACAAAGGGAGAAUCGCUGAUACCGUCGUGCCAAGAAAAGCCUCUAAGCAUAUCCAUAGUCGUCCGUACCGCAAACCGACACAGGUAGAUGAGAUGAGUAUUCUAAGGCGCGUGAAAGAACUCUGGUUAAGGAACUCUGCAAACUAGCACCGUAAGUUCGCGAUAAGGUGUGCCACAGCGAUGUGGUCUCAGCAAAGAGUCCCUCCCGACUGUUUACCAAAAACACAGCACUUUGCCAACUCGUAAGAGGAAGUAUAAGGUGUGACGCCUGCCCGGUGCUCGAAGGUUAAGAGGAUGCGUCAGUCGCAAGAUGAAGCGUUGAAUUGAAGCCCGAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGUUAAAUACCGACCUGCAUGAAUGGCGUAACGAGAUGGGAGCUGUCUCAACCAGAGAUUCAGUGAAAUUGUAGUGGAGGUGAAAAUUCCUCCUACCCGCGGCAAGACGGAAAGACCCCGUGGACCUUUACUACAACUUAGCACUGCUAAUGGGAAUAUCAUGCGCAGGAUAGGUGGGAGGCUUUGAAGUAAGGGCUUUGGCUCUUAUGGAGUCAUCCUUGAGAUACCACCCUUGAUGUUUCUGUUAGCUAACUGGCCUGUGUUAUCCACAGGCAGGACAAUGCUUGGUGGGUAGUUUGACUGGGGCGGUCGCUCCUAAAAAGUAACGGAGGCUUGCAAAGGUUGGCUCAUUGCGGUUGGAAAUCGCAAGUUGAGUGUAAUGGCACAAGCCAGCCUGACUGUAAGACAUACAAGUCAAGCAGAGACGAAAGUCGGUCAUAGUGAUCCGGUGGUUCUGUGUGGAAGGGCCAUCGCUCAAAGGAUAAAAGGUACCCCGGGGAUAACAGGCUGAUCUCCCCCAAGAGCUCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGGAGCAGGUCCCAAGGGUAUGGCUGUUCGCCAUUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCUGCCGUGGGCGUAGGAAAGUUGAGGAGAGCUGUCCCUAGUACGAGAGGACCGGGAUGGACGUGUCACUGGUGCACCAGUUGUCUGCCAAGAGCAUCGCUGGGUAGCACACACGGAUGUGAUAACUGCUGAAAGCAUCUAAGCAGGAACCAACUCCAAGAUAAACUUUCCCUGAAGCUCGCACAAAGACUAUGUGCUUGAUAGGGUAGAUGUGUGAGCGCAGUAAUGCGUUUAGCUGACUACUACUAAUAGAGCGUUUGGCUUGUUUUU";
//string s13 = "GAUUAAGUUAUUAAGGGCGCACGGUGGAUGCCUUGGCACUAGAAGCCGAUGAAGGACGUUACUAACGACGAUAUGCUUUGGGGAGCUGUAAGUAAGCUUUGAUCCAGAGAUUUCCGAAUGGGGAAACCCAGCAUGAGUUAUGUCAUGUUAUCGAUAUGUGAAUACAUAGCAUAUCAGAAGGCACACCCGGAGAACUGAAACAUCUUAGUACCCGGAGGAAGAGAAAGAAAAUUCGAUUCCCUUAGUAGCGGCGAGCGAAACGGGAAGAGCCCAAACCAACAAGCUUGCUUGUUGGGGUUGUAGGACACUCUAUACGGAGUUACAAAGGACGACAUUAGACGAAUCAUCUGGAAAGAUGAAUCAAAGAAGGUAAUAAUCCUGUAGUCGAAAAUGUUGUCUCUCUUGAGUGGAUCCUGAGUACGACGGAGCACGUGAAAUUCCGUCGGAAUCUGGGAGGACCAUCUCCUAAGGCUAAAUACUCUCUAGUGACCGAUAGUGAACCAGUACCGUGAGGGAAAGGUGAAAAGCACCCCGGAAGGGGAGUGAAAUAGAACCUGAAACCGUGUGCUUACAAGUAGUCAGAGCCCGUUAAUGGGUGAUGGCGUGCCUUUUGUAGAAUGAACCGGCGAGUUACGAUUUGAUGCAAGGUUAAGCAGUAAAUGUGGAGCCGUAGCGAAAGCGAGUCUGAAUAGGGCGUUUAGUAUUUGGUCGUAGACCCGAAACCAGGUGAUCUACCCUUGGUCAGGUUGAAGUUCAGGUAACACUGAAUGGAGGACCGAACCGACUUACGUUGAAAAGUGAGCGGAUGAACUGAGGGUAGCGGAGAAAUUCCAAUCGAACCUGGAGAUAGCUGGUUCUCUCCGAAAUAGCUUUAGGGCUAGCCUCAAGUGAUGAUUAUUGGAGGUAGAGCACUGUUUGGACGAGGGGCCCCUCUCGGGUUACCGAAUUCAGACAAACUCCGAAUGCCAAUUAAUUUAACUUGGGAGUCAGAACAUGGGUGAUAAGGUCCGUGUUCGAAAGGGAAACAGCCCAGACCACCAGCUAAGGUCCCAAAAUAUAUGUUAAGUGGAAAAGGAUGUGGCGUUGCCCAGACAACUAGGAUGUUGGCUUAGAAGCAGCCAUCAUUUAAAGAGUGCGUAAUAGCUCACUAGUCGAGUGACACUGCGCCGAAAAUGUACCGGGGCUAAACAUAUUACCGAAGCUGUGGAUUGUCCUUUGGACAAUGGUAGGAGAGCGUUCUAAGGGCGUUGAAGCAUGAUCGUAAGGACAUGUGGAGCGCUUAGAAGUGAGAAUGCCGGUGUGAGUAGCGAAAGACGGGUGAGAAUCCCGUCCACCGAUUGACUAAGGUUUCCAGAGGAAGGCUCGUCCGCUCUGGGUUAGUCGGGUCCUAAGCUGAGGCCGACAGGCGUAGGCGAUGGAUAACAGGUUGAUAUUCCUGUACCACCUAUAAUCGUUUUAAUCGAUGGGGGGACGCAGUAGGAUAGGCGAAGCGUGCGAUUGGAUUGCACGUCUAAGCAGUAAGGCUGAGUAUUAGGCAAAUCCGGUACUCGUUAAGGCUGAGCUGUGAUGGGGAGAAGACAUUGUGUCUUCGAGUCGUUGAUUUCACACUGCCGAGAAAAGCCUCUAGAUAGAAAAUAGGUGCCCGUACCGCAAACCGACACAGGUAGUCAAGAUGAGAAUUCUAAGGUGAGCGAGCGAACUCUCGUUAAGGAACUCGGCAAAAUGACCCCGUAACUUCGGGAGAAGGGGUGCUCUUUAGGGUUAACGCCCAGAAGAGCCGCAGUGAAUAGGCCCAAGCGACUGUUUAUCAAAAACACAGGUCUCUGCUAAACCGUAAGGUGAUGUAUAGGGGCUGACGCCUGCCCGGUGCUGGAAGGUUAAGAGGAGUGGUUAGCUUCUGCGAAGCUACGAAUCGAAGCCCCAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGGUAAGUUCCGACCCGCACGAAAGGCGUAACGAUUUGGGCACUGUCUCAACGAGAGACUCGGUGAAAUCAUAGUACCUGUGAAGAUGCAGGUUACCCGCGACAGGACGGAAAGACCCCGUGGAGCUUUACUGUAGCCUGAUAUUGAAAUUCGGCACAGCUUGUACAGGAUAGGUAGGAGCCUUUGAAACGUGAGCGCUAGCUUACGUGGAGGCGCUGGUGGGAUACUACCCUAGCUGUGUUGGCUUUCUAACCCGCACCACUUAUCGUGGUGGGAGACAGUGUCAGGCGGGCAGUUUGACUGGGGCGGUCGCCUCCUAAAAGGUAACGGAGGCGCUCAAAGGUUCCCUCAGAAUGGUUGGAAAUCAUUCAUAGAGUGUAAAGGCAUAAGGGAGCUUGACUGCGAGACCUACAAGUCGAGCAGGGUCGAAAGACGGACUUAGUGAUCCGGUGGUUCCGCAUGGAAGGGCCAUCGCUCAACGGAUAAAAGCUACCCCGGGGAUAACAGGCUUAUCUCCCCCAAGAGUUCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGUAGUCGGUCCCAAGGGUUGGGCUGUUCGCCCAUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCCGUCGUGGGCGUAGGAAAUUUGAGAGGAGCUGUCCUUAGUACGAGAGGACCGGGAUGGACAUACCUCUGGUGUACCAGUUGUCGUGCCAACGGCAUAGCUGGGUAGCUAUGUGUGGACGGGAUAAGUGCUGAAAGCAUCUAAGCAUGAAGCCCCCCUCAAGAUGAGAUUUCCCAACUUCGGUUAUAAGAUCCCUCAAAGAUGAUGAGGUUAAUAGGUUCGAGGUGGAAGCAUGGUGACAUGUGGAGCUGACGAAUACUAAUCGAUCGAAGACUUAAUCAA";
//string s14 = "UCAUAACGUUUGGGUAAUCGCUGCAACGCCAACGUUGUAGAGGAAAGUCCAUGCUCGCACGGCCUGAGAUGGCUGUAGUGUUCGUGCCUAGCCAAUUCAUAAGCUAGGGUAUUCUGGCUGUAAGGCUGGUUUAACGGCAGGGAAAAAACCUAAGUCCUUUCGGAUAUGGUUUGACUACCUUUAAAGUGCCACAGUGACGAAGUCCUUGAAGAAAUGAUAGGAGUGGAACGAGGUAAACCCCACGAGCGAGAAACCCAAAUAAUGGUAGGGGAAUCUUUUCCAAGGAAAUGAACGAUGGGAAAGGACAGGUUGUAUAACCUGUAGAUAGAUGAUUGCCACCGGAGUACGAGGCGUGGGCCGUUUGCAGUACAAAGGAACAGAACAUGGCUUACAGAACGUUAUGAACCA";
string s15 = "GGUUAAGUUAGAAAGGGCGCACGGUGGAUGCCUUGGCACUAGGAGCCGAUGAAGGACGGGACGAACACCGAUAUGCUUCGGGGAGCUGUAAGCAAGCUUUGAUCCGGAGAUUUCCGAAUGGGGAAACCCACCACUCGUAAUGGAGUGGUAUCCAUAUCUGAAUUCAUAGGAUAUGAGAAGGCAGACCCGGGGAACUGAAACAUCUAAGUACCCGGAGAAGAGAAAGCAAAUGCGAUUCCCUGAGUAGCGGCGACGAACACGGGAUCAGCCCAAACCAAGAGGCUUGCCUCUGUGGUUGUAGGACACUCUGUACGGAGUUACAAAAGAACGAGGUAGAUGAAGAGGUCUGGAAAGGGCCCGCCAUAGGAGGUAACAGCCCUGUAGUCAAAACUUCGUUCUCUCCUGAGUGGAUCCUGAGUACGGCGGAACACGUGAAAUUCCGUCGGAAUCCGGGAGGACCAUCUCCCAAGGCUAAAUACUCCCUAGUGACCGAUAGUGAACCAGUACCGUGAGGGAAAGGUGAAAAGCACCCCGGAAGGGGAGUGAAAGAGAUCCUGAAACCGUGUGCCUACAAGUAGUCAGAGCCCGUUAACGGUGAUGGCGUGCCUUUUGUAGAAUGAACCGGCGAGUUACGAUCCCGUGCAAGGUUAAGCAGAAGAUGCGGAGCCGCAGCGAAAGCGAGUCUGAAUAGGGCGCAUGAGUACGUGGUCGUAGACCCGAAACCAGGUGAUCUACCCAUGUCCAGGGUGAAGUUCAGGUAACACUGAAUGGAGGCCCGAACCCACGCACGUUGAAAAGUGCGGGGAUGAGGUGUGGGUAGGGGUGAAAUGCCAAUCGAACCUGGAGAUAGCUGGUUCUCUCCGAAAUAGCUUUAGGGCUAGCCUCAAGGUAAGAGUCUUGGAGGUAGAGCACUGAUUGGACUAGGGGCCCUCACCGGGUUACCGAAUUCAGUCAAACUCCGAAUGCCAAUGACUUAUCCUUGGGAGUCAGACUGCGAGUGAUAAGAUCCGUAGUCGAAAGGGAAACAGCCCAGACCGCCAGCUAAGGUCCCAAAGUAUACGUUAAGUGGAAAAGGAUGUGGAGUUGCUUAGACAACCAGGAUGUUGGCUUAGAAGCAGCCACCAUUUAAAGAGUGCGUAAUAGCUCACUGGUCGAGUGACUCUGCGCCGAAAAUGUACCGGGGCUAAACGUAUCACCGAAGCUGCGGACUGUUCUUCGAACAGUGGUAGGAGAGCGUUCUAAGGGCUGUGAAGCCAGACCGGAAGGACUGGUGGACGGCUUAGAAGUGAGAAUGCCGGUAUGAGUAGCGAAAAGAGGGGUGAGAAUCCCUCCACCGAAUGCCUAAGGGUUCCUGAGGAAGGCUCGUCCGCUCAGGGUUAGUCGGGACCUAAGCCGAGGCCGAAAGGCGUAGGCGAUGGACAACAGGUUGAUAUUCCUGUACCACCUCCUCACCAUUUGAGCAAUGGGGGGUCGCAGGAGGAUAGGGUAAGCGCGGUAUUGGAUAUCCGCGUCCAAGCAGUUAGGCUGGGAAAUAGGCAAAUCCGUUUCCCAUAAGGCUGAGCUGUGAUGGCGAGCGAAAUAUAGUAGCGAAGUUCCUGAUUCCACACUGCCAAGAAAAGCCUCUAGCGAGGUGAGAGGUGCCCGUACCGCAAACCGUCACAGGUAGGCGAGGAGAGAAUCCUAAGGUGAUCGAGAGAACUCUCGUUAAGGAACUCGGCAAAAUGACCCCGUAACUUCGGGAGAAGGGGUGCUCUGUUAGGGUGCAAGCCCGAGAGAGCCGCAGUGAAUAGGCCCAGGCGACUGUUUAGCAAAAACACAGGUCUCUGCGAAGCCGUAAGGCGAAGUAUAGGGGCUGACGCCUGCCCGGUGCUGGAAGGUUAAGAGGAGCGCUUAGCGUAAGCGAAGGUGCGAAUUGAAGCCCCAGUAAACGGCGGCCGUAACUAUAACGGUCCUAAGGUAGCGAAAUUCCUUGUCGGGUAAGUUCCGACCCGCACGAAAGGCGCAACGAUCUGGGCGCUGUCUCAACGAGAGACUCGGUGAAAUUAUAGUACCUGUGAAGAUGCAGGUUACCCGCGACAGGACGGAAAGACCCCGUGGAGCUUUACUGCAGCCUGAUAUUGAAUGUUGGUACAGCUUGUACAGGAUAGGUAGGAGCCUUGGAAACCGGAGCGCCAGCUUCGGUGGAGGCAUCGGUGGGAUACUACCCUGGCUGUAUUGACCUUCUAACCCCCCGCCCUUAUCGGGCGGGGAGACAGUGUCAGGUGGGCAGUUUGACUGGGGCGGUCGCCUCCUAAAAGGUAACGGAGGCGCCCAAAGGUUCCCUCAGAAUGGUUGGAAAUCAUUCGCAGAGUGUAAAGGCACAAGGGAGCUUGACUGCGAGACCUACAAGUCGAGCAGGGACGAAAGUCGGGCUUAGUGAUCCGGUGGUUCCGCAUGGAAGGGCCAUCGCUCAACGGAUAAAAGCUACCCCGGGGAUAACAGGCUUAUCUCCCCCAAGAGCUCCACAUCGACGGGGAGGUUUGGCACCUCGAUGUCGGCUCAUCGCAUCCUGGGGCUGUAGUCGGUCCCAAGGGUUGGGCUGUUCGCCCAUUAAAGCGGUACGCGAGCUGGGUUCAGAACGUCGUGAGACAGUUCGGUCCCUAUCCGUCGCGGGCGCUGGAAAUUUGAGAGGAGCUGUCCUUAGUACGAGAGGACCGGGAUGGACGCACCGCUGGUGUACCAGUUGUUCUGCCAAGGGCAUCGCUGGGUAGCUAUGUGCGGACGGGAUAAGUGCUGAAAGCAUCUAAGCAUGAAGCCCCCCUCAAGAUGAGAUUUCCCAUUCCGCAAGGAAGUAAGAUCCCUGAAAGAUGAUCAGGUUGAUAGGUCUGAGGUGGAAGUGUGGCAACACAUGGAGCUGACAGAUACUAAUCGAUCGAGGACUUAACCAU";

vector<string> testData{s1,s2,s3,s4,s5,s6,s7,s8};




/** This creates the data that is run by our functions. The minimum size for the data is 30	**/
void createData(vector<string> &testData, int NOS, int maxSize){


	const vector<char> bases = {'A','C','G','T'}; 


	for(int i = 0; i<NOS;++i){

	string sequence = "";
	int treshold = 1;
	int size = rand()%(maxSize-30) + 30;

		for(int j = 0; j<size;j++){

		int random = rand()%4;
		char base = bases[random];
		sequence = sequence + base;

		}
		//cout<< sequence << '\n'<<endl;

	testData.push_back(sequence);
	}
}

