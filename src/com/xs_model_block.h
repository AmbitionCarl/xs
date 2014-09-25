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
#ifndef __XS_MODEL_BLOCK_H__
#define __XS_MODEL_BLOCK_H__

int xs_model_send_arg(int fd, int argc, ...);
int xs_model_recv_arg(int fd, int argc, ...);

int xs_model_send_block(xs_model_t* model, int fd, int timeout);
int xs_model_recv_block(xs_model_t** model, int fd, int timeout);


#endif
#ifdef __cplusplus
}
#endif
