#include "grpc_checkpoints_service.h"

#include <unistd.h>
#include <iostream>
#include <memory>
#include <new>
#include <string>

#include "isula_libutils/log.h"
#include "utils.h"
#include "grpc_server_tls_auth.h"



//获取请求
int CheckpointServiceImpl::checkpoint_create_request_from_grpc(const CreateCheckpointRequest *grequest,
                                                      checkpoint_create_checkpoint_request **request)
{
    checkpoint_create_checkpoint_request *tmpreq =
        static_cast<checkpoint_create_checkpoint_request *>(util_common_calloc_s(sizeof(checkpoint_create_checkpoint_request)));
    if (tmpreq == nullptr) {
        ERROR("Out of memory");
        return -1;
    }
    *request = tmpreq;

    return 0;
}

//发送响应
int CheckpointServiceImpl::checkpoint_create_response_to_grpc(checkpoint_create_checkpoint_response *response,
                                                     CreateCheckpointResponse *gresponse)
{
    if (response == nullptr) {
        gresponse->set_cc(ISULAD_ERR_MEMOUT);
        return 0;
    }

    gresponse->set_cc(response->cc);
    if (response->errmsg != nullptr) {
        gresponse->set_errmsg(response->errmsg);
    }

   // for (size_t i {}; i < response->checkpoints_len; i++) {
     //   gresponse->add_checkpoints(response->checkpoints[i]);
    //}

    return 0;
}


//处理逻辑
Status CheckpointServiceImpl::Create(ServerContext *context, const CreateCheckpointRequest *request, CreateCheckpointResponse *reply)
{
    //tls认证
    auto status = GrpcServerTlsAuth::auth(context, "checkpoint_create");
    if (!status.ok()) {
        return status;
    }
    //获取服务执行器
    service_executor_t *cb = get_service_executor();
    //获取为空
    if (cb == nullptr || cb->checkpoint.create == nullptr) {
        return Status(StatusCode::UNIMPLEMENTED, "Unimplemented callback");
    }
    //声明请求
    checkpoint_create_checkpoint_request *checkpoint_req = nullptr;
    //填充请求
    int tret = checkpoint_create_request_from_grpc(request, &checkpoint_req);
    if (tret != 0) {
        ERROR("Failed to transform grpc request");
        reply->set_cc(ISULAD_ERR_INPUT);
        return Status::OK;
    }

    //声明响应
    checkpoint_create_checkpoint_response *checkpoint_res = nullptr;
    //填充响应
    int ret = cb->checkpoint.create(checkpoint_req, &checkpoint_res);
    //发送响应
    tret = checkpoint_create_response_to_grpc(checkpoint_res, reply);
    //free_checkpoint_create_checkpoint_request(checkpoint_req);
    //free_checkpoint_create_checkpoint_response(checkpoint_res);
    if (tret != 0) {
        reply->set_errmsg(util_strdup_s(errno_to_error_message(ISULAD_ERR_INTERNAL)));
        reply->set_cc(ISULAD_ERR_INPUT);
        ERROR("Failed to translate response to grpc, operation is %s", ret ? "failed" : "success");
    }

    return Status::OK;
}
