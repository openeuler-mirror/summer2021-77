#include "grpc_checkpoints_client.h"

#include <string>

#include "api.grpc.pb.h"
#include "client_base.h"
#include "checkpoints.grpc.pb.h"
#include "utils.h"

using namespace checkpoint;

using grpc::ClientContext;
using grpc::Status;


class CheckpointCreate : public
    ClientBase<CheckpointService, CheckpointService::Stub, isula_create_checkpoint_request, CreateCheckpointRequest,
    isula_create_checkpoint_response, CreateCheckpointResponse> {
public:
    explicit CheckpointCreate(void *args)
        : ClientBase(args)
    {
    }
    ~CheckpointCreate() = default;
    CheckpointCreate(const CheckpointCreate &) = delete;
    CheckpointCreate &operator=(const CheckpointCreate &) = delete;

    auto request_to_grpc(const isula_create_checkpoint_request *request, CreateCheckpointRequest *grequest) -> int override
    {
        printf("requesttogrpc\n");
        if (request == nullptr) {
            return -1;
        }

        //if (request->name != nullptr) {
        //    grequest->set_name(request->name);
        //}

        return 0;
    }

    auto response_from_grpc(CreateCheckpointResponse *gresponse, isula_create_checkpoint_response *response) -> int override
    {
        response->server_errono = static_cast<uint32_t>(gresponse->cc());

        if (!gresponse->errmsg().empty()) {
            response->errmsg = util_strdup_s(gresponse->errmsg().c_str());
        }

        return 0;
    }

    auto grpc_call(ClientContext *context, const CreateCheckpointRequest &req, CreateCheckpointResponse *reply) -> Status override
    {
        printf("grpc_call\n");
    
        return stub_->Create(context, req, reply);
    }
};

auto grpc_checkpoints_client_ops_init(isula_connect_ops *ops) -> int
{

    if (ops == nullptr) {
        return -1;
    }
    ops->checkpoint.create = container_func<isula_create_checkpoint_request, isula_create_checkpoint_response, CheckpointCreate>;

    return 0;
}
