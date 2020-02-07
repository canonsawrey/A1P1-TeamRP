#pragma once

#include "helper.h"
#include "getopt.h"
#include <limits.h>

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
        unsigned int argCount = 0;
        unsigned int printColIndexCol = 0;
        unsigned int printColIndexOffset = 0;
        unsigned int isMissingCol = 0;
        unsigned int isMissingOffset = 0;
        
        FlagConfig(int argc, char** argv) {
            //set opterr to zero so we can provide a custom error message for unrecognized flags
            extern int opterr;
            opterr = 0;
            int opt;

            int long_index = 0;
            while ((opt = getopt_long_only(argc, argv,"", 
                        long_options, &long_index)) != -1) {
                switch (opt) {
                    // filename
                    case 'f':
                        filename = optarg;
                        break;
                    // -from = bytes to read from
                    case 'r':
                        if (has_only_digits(optarg)) {
                            from = stoul(optarg);
                        } else {
                            fail("Invalid argument for -from. Program terminated.");
                        }
                        break;
                    // -len = length
                    case 'l':
                        if (has_only_digits(optarg)) {
                            len = stoul(optarg);
                        } else {
                            fail("Invalid argument for -len. Program terminated.");
                        }
                        break;
                    // -print_col_type
                    case 't':
                        if (has_only_digits(optarg)) {
                            printColType = stoul(optarg);
                        }
                        break;
                    // -print_col_idx
                    case 'i':
                        if (has_only_digits(optarg)) {
                            printColIndexCol = stoul(optarg);
                            argCount++;
                        }
                        // ensure there are two uints given
                        for( ;optind < argc && *argv[optind] != '-'; optind++){
                            if (has_only_digits(argv[optind])) {
                                printColIndexOffset = stoul(argv[optind]);
                                printCell = true;
                                argCount++;
                            }
                        }
                        if (!printCell || argCount != 2) {
                            fail("Invalid arguments for print_col_idx. Two uints required. Program terminated.");
                        }
                        argCount = 0;
                        break;
                    // -is_missing_idx
                    case 'm':
                        if (has_only_digits(optarg)) {
                            isMissingCol = stoul(optarg);
                            argCount++;
                        }
                        // ensure there are two uints given
                        for( ;optind < argc && *argv[optind] != '-'; optind++){
                            if (has_only_digits(argv[optind])) {
                                isMissingOffset = stoul(argv[optind]);
                                printIsMissing = true;
                                argCount++;
                            }
                        }
                        if (!printIsMissing || argCount != 2) {
                            fail("Invalid arguments for is_missing_idx. Two uints required. Program terminated.");
                        }
                        argCount = 0;
                        break;
                    // unrecognized flag
                    default:
                        fail("Unrecognized flag. Program terminated.");
                }
            }
        }
};