
#pragma once

#include "arpc_connector.h"
#include <functional>
#include <map>
#include <vector>


class Setting {
public:
    std::map<int, Connector *> Conns;

    Setting() {}
};
