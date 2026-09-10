#define SHU_IMPLEMENTATION
#include <shuarg.h>

void help(const char *message)
{
    SHU_LogInfo(
        "%s : Here is the help:\n\
        'وَمَنْ يَتَّقِ اللّٰهَ يَجْعَلْ لَهُ مَخْرَجًاۙ وَيَرْزُقْهُ مِنْ حَيْثُ لَا يَحْتَسِبُۜ وَمَنْ يَتَوَكَّلْ عَلَى اللّٰهِ فَهُوَ حَسْبُهُۜ اِنَّ اللّٰهَ بَالِغُ اَمْرِه۪ۜ قَدْ جَعَلَ اللّٰهُ لِكُلِّ شَيْءٍ قَدْرًا'\n\
        'And whoever is mindful of Allah, He will make a way out for them, and provide for them from \
        sources they could never imagine. And whoever puts their trust in Allah, then He ˹alone˺ is \
        sufficient for them. Certainly Allah achieves His Will. Allah has already set a destiny for everything.'",
        message);
}

int main(int argc, char **argv)
{
    bool mandatory;

    args(argc, argv)
    {
        bool retHelp = argBoolean("-h");

        if (retHelp)
        {
            help("help?");
        }

        mandatory = argBoolean("-foo", help("mandatory is mandatory"););
    }

    return 0;
}