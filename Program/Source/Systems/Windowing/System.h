/**
 * @file System.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for the creation / initialization,
 * destruction / deinitialization, and other management tasks to do with
 * within both the Linux and Windows windowing systems.
 * @date 2024-08-05
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef _MSENG_SYSTEM_WINDOWING_SYSTEM_
#define _MSENG_SYSTEM_WINDOWING_SYSTEM_

// The master include file for the project.
#include <Master.h>
#include <Utilities/WaylandTypes.h>

void SetupWindowingSystem(void);
void DestroyWindowingSystem(void);

bool CheckDisplayServer(void);

registry_t* GetRegistry(void);

#endif // _MSENG_SYSTEM_WINDOWING_SYSTEM_
