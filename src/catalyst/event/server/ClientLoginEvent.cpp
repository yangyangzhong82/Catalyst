#include "ClientLoginEvent.h"

#include "ll/api/event/Emitter.h"
#include "ll/api/event/EventBus.h"
#include "ll/api/memory/Hook.h"
#include "mc/certificates/identity/PlayerAuthenticationInfo.h"
#include "mc/network/ConnectionRequest.h"
#include "mc/network/NetworkIdentifier.h"
#include "mc/network/ServerConnectionAuthValidator.h"
#include "mc/network/ServerNetworkHandler.h"


#include "mc/network/packet/LoginPacket.h"

#include <variant>
