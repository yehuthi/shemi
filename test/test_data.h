#pragma once
// Test data in a separate file because editors might really hate these
// characters.

static const char32_t HEBR[] = U"אבגדהוזחטיךכלםמןנסעףפץצקרשת";
static const size_t HEBR_LEN = sizeof(HEBR) / sizeof(HEBR[0]) - 1;
static const char32_t PHNX[] = U"𐤀𐤁𐤂𐤃𐤄𐤅𐤆𐤇𐤈𐤉𐤊𐤋𐤌𐤍𐤎𐤏𐤐𐤑𐤒𐤓𐤔𐤕";
static const size_t PHNX_LEN = sizeof(PHNX) / sizeof(PHNX[0]) - 1;
static const char32_t ARMI[] = U"𐡀𐡁𐡂𐡃𐡄𐡅𐡆𐡇𐡈𐡉𐡊𐡋𐡌𐡍𐡎𐡏𐡐𐡑𐡒𐡓𐡔𐡕";
static const char32_t SAMR[] = U"ࠀࠁࠂࠃࠄࠅࠆࠇࠈࠉࠊࠋࠌࠍࠎࠏࠐࠑࠒࠓࠔࠕ";
static const char32_t FOREIGN[] = U"A\n ʃʰߞि֯५Ⅸ௰_–«»؟״∞₿˘⚔ʬ⍥⧗⸘⿻⽕㊙㍿"
	U"🛰🌌🧬⚛ʒ₨∤⇌Ϟ☯ℤ𝒜⛷𐎀𑀓꧅𓂀𓇋𒀱ᚠ🐲♘🎭🚀💥🍜💎🪐🧠☘︎🎮⚖☄⎋⌬⧉∮";
static const size_t FOREIGN_LEN = sizeof(FOREIGN) / sizeof(FOREIGN[0]);

static const char32_t PHNX_FROM_HEBREW[] = U"𐤀𐤁𐤂𐤃𐤄𐤅𐤆𐤇𐤈𐤉𐤊𐤊𐤋𐤌𐤌𐤍𐤍𐤎𐤏𐤐𐤐𐤑𐤑𐤒𐤓𐤔𐤕";
static const char32_t ARMI_FROM_HEBREW[] = U"𐡀𐡁𐡂𐡃𐡄𐡅𐡆𐡇𐡈𐡉𐡊𐡊𐡋𐡌𐡌𐡍𐡍𐡎𐡏𐡐𐡐𐡑𐡑𐡒𐡓𐡔𐡕";
static const char32_t SAMR_FROM_HEBREW[] =
	U"ࠀࠁࠂࠃࠄࠅࠆࠇࠈࠉࠊࠊࠋࠌࠌࠍࠍࠎࠏࠐࠐࠑࠑࠒࠓࠔࠕ";