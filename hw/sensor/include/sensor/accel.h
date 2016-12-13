/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef __SENSOR_ACCEL_H__
#define __SENSOR_ACCEL_H__

#include "os/os.h"
#include "os/os_dev.h"
#include "sensor/sensor.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Data representing a singular read from an accelerometer.
 * All values are in MS^2
 */
struct sensor_accel_data {
    int32_t sad_x;
    int32_t sad_y;
    int32_t sad_z;
} __attribute__((packed));

/**
 * Accelerometer data is unused for this field.
 */
#define SENSOR_ACCEL_DATA_UNUSED (0xFFFFFFFF)

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_ACCEL_H__ */
