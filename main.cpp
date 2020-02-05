#include "sor.h"
#include "flag_config.h"
#include <limits.h>

using namespace std;

int main(int argc, char** argv) {

    FlagConfig configuration = FlagConfig(argc, argv);

    Sor * sorProcessor = new Sor(configuration.from, configuration.len, configuration.filename);
    //if -print_col_type input was valid
    if (configuration.printColType != UINT_MAX) {
        sorProcessor->printColumnType(configuration.printColType);
    }
    
    //if -print_col_idx input was valid
    if (configuration.printCell) {
        sorProcessor->printValue(configuration.printColIndexCol, configuration.printColIndexOffset);
    }

    //if -is_missing_idx input was valid
    if (configuration.printIsMissing) {
        sorProcessor->printIsMissing(configuration.isMissingCol, configuration.isMissingOffset);
    }

    return 0;
}