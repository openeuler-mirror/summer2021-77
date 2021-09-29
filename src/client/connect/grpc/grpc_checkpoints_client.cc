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

        if (request->container != nullptr) {
            grequest->set_container(request->container);
        }
        if (request->dir != nullptr) {
            grequest->set_dir(request->dir);
        }

        return 0;
    }

    auto response_from_grpc(CreateCheckpointResponse *gresponse, isula_create_checkpoint_response *response) -> int override
    {
        response->server_errono = static_cast<uint32_t>(gresponse->cc());

        if (!gresponse->errmsg().empty()) {
            response->errmsg = util_strdup_s(gresponse->errmsg().c_str());
        }
        response->container=util_strdup_s(gresponse->container().c_str());

        return 0;
    }

    auto grpc_call(ClientContext *context, const CreateCheckpointRequest &req, CreateCheckpointResponse *reply) -> Status override
    {
       
    
        return stub_->Create(context, req, reply);
    }
};

class CheckpointRemove : public
    ClientBase<CheckpointService, CheckpointService::Stub, isula_remove_checkpoint_request, RemoveCheckpointRequest,
    isula_remove_checkpoint_response, RemoveCheckpointResponse> {
public:
    explicit CheckpointRemove(void *args)
        : ClientBase(args)
    {
    }
    ~CheckpointRemove() = default;
    CheckpointRemove(const CheckpointRemove &) = delete;
    CheckpointRemove &operator=(const CheckpointRemove &) = delete;

    auto request_to_grpc(const isula_remove_checkpoint_request *request, RemoveCheckpointRequest *grequest) -> int override
    {
        if (request == nullptr) {
            return -1;
        }

        if (request->container != nullptr) {
            grequest->set_container(request->container);
        }
        if (request->dir != nullptr) {
            grequest->set_dir(request->dir);
        }

        return 0;
    }

    auto response_from_grpc(RemoveCheckpointResponse *gresponse, isula_remove_checkpoint_response *response) -> int override
    {
        response->server_errono = static_cast<uint32_t>(gresponse->cc());

        if (!gresponse->errmsg().empty()) {
            response->errmsg = util_strdup_s(gresponse->errmsg().c_str());
        }
        response->container=util_strdup_s(gresponse->container().c_str());
        
        return 0;
    }

    auto grpc_call(ClientContext *context, const RemoveCheckpointRequest &req, RemoveCheckpointResponse *reply) -> Status override
    {
       
    
        return stub_->Remove(context, req, reply);
    }
};

class CheckpointList : public
    ClientBase<CheckpointService, CheckpointService::Stub, isula_list_checkpoint_request, ListCheckpointRequest,
    isula_list_checkpoint_response, ListCheckpointResponse> {
public:
    explicit CheckpointList(void *args)
        : ClientBase(args)
    {
    }
    ~CheckpointList() = default;
    CheckpointList(const CheckpointList &) = delete;
    CheckpointList &operator=(const CheckpointList &) = delete;

    auto request_to_grpc(const isula_list_checkpoint_request *request, ListCheckpointRequest *grequest) -> int override
    {
        if (request == nullptr) {
            return -1;
        }

        return 0;
    }

    auto response_from_grpc(ListCheckpointResponse *gresponse, isula_list_checkpoint_response *response) -> int override
    {
        response->server_errono = static_cast<uint32_t>(gresponse->cc());

        if (!gresponse->errmsg().empty()) {
            response->errmsg = util_strdup_s(gresponse->errmsg().c_str());
        }
        response->checkpoints=util_strdup_s(gresponse->checkpoints().c_str());

        return 0;
    }

    auto grpc_call(ClientContext *context, const ListCheckpointRequest &req, ListCheckpointResponse *reply) -> Status override
    {
        return stub_->List(context, req, reply);
    }
};

class CheckpointRestore : public
    ClientBase<CheckpointService, CheckpointService::Stub, isula_restore_checkpoint_request, RestoreCheckpointRequest,
    isula_restore_checkpoint_response, RestoreCheckpointResponse> {
public:
    explicit CheckpointRestore(void *args)
        : ClientBase(args)
    {
    }
    ~CheckpointRestore() = default;
    CheckpointRestore(const CheckpointRestore &) = delete;
    CheckpointRestore &operator=(const CheckpointRestore &) = delete;

    auto request_to_grpc(const isula_restore_checkpoint_request *request, RestoreCheckpointRequest *grequest) -> int override
    {
        if (request == nullptr) {
            return -1;
        }

        return 0;
    }

    auto response_from_grpc(RestoreCheckpointResponse *gresponse, isula_restore_checkpoint_response *response) -> int override
    {
        response->server_errono = static_cast<uint32_t>(gresponse->cc());

        if (!gresponse->errmsg().empty()) {
            response->errmsg = util_strdup_s(gresponse->errmsg().c_str());
        }
        response->checkpoints=util_strdup_s(gresponse->checkpoints().c_str());

        return 0;
    }

    auto grpc_call(ClientContext *context, const RestoreCheckpointRequest &req, RestoreCheckpointResponse *reply) -> Status override
    {
        return stub_->Restore(context, req, reply);
    }
};

auto grpc_checkpoints_client_ops_init(isula_connect_ops *ops) -> int
{

    if (ops == nullptr) {
        return -1;
    }
    ops->checkpoint.create = container_func<isula_create_checkpoint_request, isula_create_checkpoint_response, CheckpointCreate>;
    ops->checkpoint.remove = container_func<isula_remove_checkpoint_request, isula_remove_checkpoint_response, CheckpointRemove>;
    ops->checkpoint.list   = container_func<isula_list_checkpoint_request  , isula_list_checkpoint_response  , CheckpointList>;
    ops->checkpoint.restore   = container_func<isula_restore_checkpoint_request  , isula_restore_checkpoint_response  , CheckpointRestore>;

    return 0;
}
