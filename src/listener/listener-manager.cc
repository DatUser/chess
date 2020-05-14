#include <listener/listener-manager.hh>

namespace listener {

ListenerManager& ListenerManager::instance() {
    static ListenerManager out = ListenerManager();
    return out;
}

}
