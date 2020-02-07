#include "sor.h"
#include "flag_config.h"
#include <limits.h>

using namespace std;

int main(int argc, char** argv) {
    // set up a configuration object
    FlagConfig configuration = FlagConfig(argc, argv);

    SorAdapter * sorAdapter = new SorAdapter(configuration.from, configuration.len, configuration.filename);
    //if -print_col_type input was valid
    if (configuration.printColType != UINT_MAX) {
        sorAdapter->printColumnType(configuration.printColType);
    }
    
    //if -print_col_idx input was valid
    if (configuration.printCell) {
        sorAdapter->printValue(configuration.printColIndexCol, configuration.printColIndexOffset);
    }

    //if -is_missing_idx input was valid
    if (configuration.printIsMissing) {
        sorAdapter->printIsMissing(configuration.isMissingCol, configuration.isMissingOffset);
    }

    delete sorAdapter;

    return 0;
}