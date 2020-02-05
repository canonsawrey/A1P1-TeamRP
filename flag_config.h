#include "helper.h"
#include "getopt.h"

//declaring the command-line flags
struct option long_options[] = {
    {"f",               required_argument,       0,  'f' },
    {"from",            required_argument,       0,  'r' },
    {"len",             required_argument,       0,  'l' },
    {"print_col_type",  required_argument,       0,  't' },
    {"print_col_idx",   required_argument,       0,  'i' },
    {"is_missing_idx",  required_argument,       0,  'm' },
    {0,                 0,                       0,   0  }
};
/**
 * Used to keep the main function clean. Takes in the command line arguments and turns them into an object
 */
struct FlagConfig {
    public:
        string filename;
        unsigned int from = 0;
        unsigned int len = UINT_MAX;
        unsigned int printColType = UINT_MAX;
        bool printCell = false;
        bool printIsMissing = false;
        unsigned int printColIndexCol = 0;
        unsigned int printColIndexOffset = 0;
        unsigned int isMissingCol = 0;
        unsigned int isMissingOffset = 0;
        
        FlagConfig(int argc, char** argv) {
            extern int opterr;
            opterr = 0;
            int opt;

            int long_index = 0;
            while ((opt = getopt_long_only(argc, argv,"", 
                        long_options, &long_index)) != -1) {
                switch (opt) {
                    case 'f':
                        filename = optarg;
                        break;
                    case 'r':
                        if (has_only_digits(optarg)) {
                            from = stoul(optarg);
                        } else {
                            throw "Invalid argument for -from. Program terminated.";
                        }
                        break;
                    case 'l':
                        if (has_only_digits(optarg)) {
                            len = stoul(optarg);
                        } else {
                            throw "Invalid argument for -len. Program terminated.";
                        }
                        break;
                    case 't':
                        if (has_only_digits(optarg)) {
                            printColType = stoul(optarg);
                        }
                        break;
                    case 'i':
                        if (has_only_digits(optarg)) {
                            printColIndexCol = stoul(optarg);
                        }
                        for( ;optind < argc && *argv[optind] != '-'; optind++){
                            if (has_only_digits(argv[optind])) {
                                printColIndexOffset = stoul(argv[optind]);
                                printCell = true;
                            }
                        }
                        if (!printCell) {
                            throw "Invalid arguments for print_col_idx. Two uints required. Exiting.";
                        }
                        break;
                    case 'm':
                        if (has_only_digits(optarg)) {
                            isMissingCol = stoul(optarg);
                        }
                        for( ;optind < argc && *argv[optind] != '-'; optind++){
                            if (has_only_digits(argv[optind])) {
                                isMissingOffset = stoul(argv[optind]);
                                printIsMissing = true;
                            }  
                        }
                        if (!printIsMissing) {
                            throw "Invalid arguments for is_missing_idx. Two uints required. Exiting.";
                        }
                        break;
                    default:
                        exit(EXIT_FAILURE);
                }
            }
        }
};