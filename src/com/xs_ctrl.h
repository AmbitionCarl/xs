
/*
 * This file is part of the xs Library.
 *
 * Copyright (C) 2011 by Guoliang Xue <xueguoliang@gmail.com>
 *
 * The xs Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The xs Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with the xs Library. If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef __cplusplus
extern "C"{
#endif
#ifndef __XS_CTRL_H__
#define __XS_CTRL_H__

/*
 * ctrl design for server 2 server 
 * user short connetion
 * */

typedef void (*xs_ctrl_recv_t)(int, struct xs_ctrl_t*);
typedef void (*xs_ctrl_handle_t)(int, char*, int, struct xs_ctrl_t*);

/* recv and handle can be temp modify */
typedef struct xs_ctrl_t
{
    xs_hash_t*          objs;
    xs_ev_sock_t*       server;
    xs_ctrl_recv_t      r;
    xs_ctrl_handle_t    h;
    uint16_t            port;
} xs_ctrl_t;

xs_ctrl_t* xs_ctrl_create(uint16_t port, xs_ctrl_recv_t r, 
        xs_ctrl_handle_t h);
void xs_ctrl_destroy(xs_ctrl_t* ctrl);

void xs_ctrl_resp(xs_ctrl_t* ctrl, int fd, int argc, ...);

#if 0
int xs_ctrl_send_block_ip(xs_ctrl_t* ctrl, char* ip, char* buf, int size);
int xs_ctrl_send_block(xs_ctrl_t* ctrl, int fd, char* buf, int size);

char* xs_ctrl_recv_block(xs_ctrl_t* ctrl, int fd, int* size);
char* xs_ctrl_recv_block_ip(xs_ctrl_t* ctrl, char* ip, int* size);
#endif

// here
void xs_ctrl_recv_df(int fd, xs_ctrl_t* ctrl);
void xs_ctrl_handle_df(int fd, char* buf, int size, xs_ctrl_t* ctrl);

#define xs_ctrl_reg_obj(ctrl, objname, obj) \
    xs_hash_add(ctrl->objs, objname, obj)
static inline void xs_ctrl_reg_handler(xs_ctrl_t* ctrl, const char* objname, const char* opname, xs_object_handler_t op)
{

    xs_object_t* obj = (xs_object_t*)xs_hash_find(ctrl->objs, objname);

    if(obj == NULL)
    {
        obj = xs_object_create();
        xs_ctrl_reg_obj(ctrl, objname, obj);
    }

    xs_hash_print(obj);
    if(xs_object_add_op(obj, opname, op) < 0)
    {
        /* add fail */
        xs_hash_del(obj, (char*)opname, NULL);
        xs_object_add_op(obj, opname, op);

        xs_logd("add object failure:%s, %s", objname, opname);
        xs_hash_print(obj);
        exit(0);
    }
    else
    {
        xs_logd("add object ok:%s, %s", objname, opname);
        xs_hash_print(obj);
    }
}
#endif


#ifdef __cplusplus
}
#endif
