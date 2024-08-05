/**
 * @file Registry.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief A short and sweet file to handle the addition and deletion of
 * registry objects.
 * @date 2024-08-05
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_REGISTRY_WINDOWING_SYSTEM_
#define _MSENG_REGISTRY_WINDOWING_SYSTEM_

#include <Master.h>
#include <Utilities/WaylandTypes.h>

void BeginRegistry(display_t* display);
void DestroyRegistry(void);
registry_t* GetRegistry(void);

#endif // _MSENG_REGISTRY_WINDOWING_SYSTEM_
