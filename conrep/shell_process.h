/* 
 * Copyright 2007-2013 Howard Jeng <hjeng@cowfriendly.org>
 * 
 * This file is part of Conrep.
 * 
 * Conrep is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * Eraser is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * A copy of the GNU General Public License can be found at
 * <http://www.gnu.org/licenses/>.
 */

// Interface for interacting with the shell process and its window

#ifndef CONREP_SHELL_PROCESS_H
#define CONREP_SHELL_PROCESS_H

#include "windows.h"

#include <memory>
#include "atl.h"
#include "tchar.h"

namespace console {
  struct Dimension;
  struct Settings;
  class CharInfoBuffer;
  class ProcessLock;
  
  // ProcessLock should be considered to be part of the public interface of 
  //   ShellProcess. Since there are function calls in the ShellProcess
  //   that should only be called between attach() and detach() calls, we
  //   enforce that restriction with the type system by making those calls
  //   only possible through the ProcessLock class. It also guarantees the
  //   call of ShellProcess::detach() in the event of an exception.
  class ShellProcess {
    public:
      ShellProcess(Settings & settings);
      ~ShellProcess();

      bool attached(void);
        
      HWND window_handle(void) const;
      HANDLE process_handle(void) const;

      bool is_console_visible(void) const;
      void toggle_console_visible(void);
    private:
      ShellProcess(const ShellProcess &);
      ShellProcess & operator=(const ShellProcess &);
        
      CHandle process_handle_;
      CHandle stdout_handle_;
      DWORD   process_id_;
      HWND    window_handle_;
      bool    console_visible_; // if the console associated with the shell process is visible
        
      static int attach_count_;

      bool attach(void);
      void detach(void);
        
      void create_shell_process(Settings & settings);
        
      void get_console_info(const Dimension & console_dim, CharInfoBuffer & buffer, COORD & cursor_pos);
      Dimension resize(Dimension console_dim);

      Dimension get_console_size(void);
        
      void reset_handle(void);

      friend ProcessLock;
  };
  
  class ProcessLock {
    public:
      ProcessLock(ShellProcess & shell_process);
      ~ProcessLock();

      Dimension resize(Dimension console_dim);
      void get_console_info(const Dimension & console_dim, CharInfoBuffer & buffer, COORD & cursor_pos);
      Dimension get_console_size(void);

      operator bool(void) const;
    private:
      ShellProcess & shell_process_;
      bool attached_;
      
      ProcessLock(const ProcessLock &);
      ProcessLock & operator=(const ProcessLock &);
  };
}

#endif