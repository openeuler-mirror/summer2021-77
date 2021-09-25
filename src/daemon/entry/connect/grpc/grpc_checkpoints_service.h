#ifndef DAEMON_ENTRY_CONNECT_GRPC_GRPC_VOLUMES_SERVICE_H
#define DAEMON_ENTRY_CONNECT_GRPC_GRPC_VOLUMES_SERVICE_H

#include <grpc++/grpc++.h>

#include "checkpoints.grpc.pb.h"
#include "callback.h"
#include "error.h"

using grpc::ServerContext;
using grpc::Status;

using namespace checkpoint;

// Implement of checkpoint service
class CheckpointServiceImpl final : public CheckpointService::Service {
public:
    CheckpointServiceImpl() = default;
    CheckpointServiceImpl(const CheckpointServiceImpl &) = delete;
    CheckpointServiceImpl &operator=(const CheckpointServiceImpl &) = delete;
    virtual ~CheckpointServiceImpl() = default;

    Status Create(ServerContext *context, const CreateCheckpointRequest *request, CreateCheckpointResponse *reply) override;

private:
    template <class T1, class T2>
    int response_to_grpc(const T1 *response, T2 *gresponse)
    {
        if (response == nullptr) {
            gresponse->set_cc(ISULAD_ERR_MEMOUT);
            return 0;
        }
        gresponse->set_cc(response->cc);
        if (response->errmsg != nullptr) {
            gresponse->set_errmsg(response->errmsg);
        }
        return 0;
    }

    int checkpoint_create_request_from_grpc(const CreateCheckpointRequest *grequest, checkpoint_create_checkpoint_request **request);

    int checkpoint_create_response_to_grpc(checkpoint_create_checkpoint_response *response, CreateCheckpointResponse *gresponse);
};

#endif // DAEMON_ENTRY_CONNECT_GRPC_GRPC_VOLUMES_SERVICE_H
