typedef long  _INTEGER;
typedef char* _ADDRESS;
typedef void (*_PROC)();

typedef struct module {
  _ADDRESS  file;
  _ADDRESS  type_cells;
  _ADDRESS  type_cell_ptrs;
  _ADDRESS  full_revelations;
  _ADDRESS  partial_revelations;
  _ADDRESS  proc_info;
  _ADDRESS  try_scopes;
  _ADDRESS  var_map;
  _ADDRESS  gc_map;
  _PROC     link;
  _PROC     main;
} _MODULE;

typedef struct link_info {
  _INTEGER n_modules;
  _ADDRESS modules;
  _INTEGER argc;
  _ADDRESS argv;
  _ADDRESS envp;
  _ADDRESS instance;
  _ADDRESS bottom_of_stack;
  _ADDRESS top_of_stack;
} _LINK_INFO;

typedef struct {
  _MODULE     module;
  _ADDRESS    info_typecell[26];
  _LINK_INFO *info;
} _LINKER;


struct {
  struct { int DatePosix; } Date;
  struct { int FSPosix; } FS;
  struct { int FmtBuf; } FmtBufF;
  struct { int FmtBuf; } FmtBufTest;
  struct { int MachineIDPosix; } MachineID;
  struct { int unibm; } Main;
  struct { int OSErrorPosix; } OSError;
  struct { int PathnamePosix; } Pathname;
  struct { int PipePosix; int Pipe; } Pipe;
  struct { int ProcessPosix; } Process;
  struct { int RTCollector; } RTCollectorSRC;
  struct { int RTException; } RTExRep;
  struct { int RTHeapRep; int RTCollector; } RTHeapRep;
  struct { int ThreadPosix; int RTHooks; int RTAllocator; } RTHooks;
  struct { int RTProcedure; } RTProcedureSRC;
  struct { int RTThreadStk; int RTThread; } RTThread;
  struct { int ThreadPosix; } RTThreadInit;
  struct { int RTType; } RTTypeSRC;
  struct { int RTCollector; } RTWeakRef;
  struct { int RdMove; int RdImpl; } Rd;
  struct { int RdMove; } RdClass;
  struct { int ThreadPosix; } Scheduler;
  struct { int ThreadPosix; } SchedulerPosix;
  struct { int UnsafeHash; int Text; } Text;
  struct { int Text; } TextF;
  struct { int ThreadPosix; } Thread;
  struct { int ThreadPosix; } ThreadF;
  struct { int TickPortable; } Tick;
  struct { int TimePosix; } Time;
  struct { int RdMove; } UnsafeRd;
  struct { int WrMove; } UnsafeWr;
  struct { int WrPosix; int WrMove; } Wr;
  struct { int WrMove; } WrClass;
} * _m3_exporters;

extern _MODULE MM_Atom;
extern _MODULE MI_Atom;
extern _MODULE MI_AtomAtomTbl;
extern _MODULE MM_AtomAtomTbl;
extern _MODULE MM_AtomList;
extern _MODULE MI_AtomList;
extern _MODULE MI_BasicCtypes;
extern _MODULE MM_CConvert;
extern _MODULE MI_CConvert;
extern _MODULE MI_Cerrno;
extern _MODULE MM_Convert;
extern _MODULE MI_Convert;
extern _MODULE MI_Csetjmp;
extern _MODULE MI_Cstddef;
extern _MODULE MI_Cstdlib;
extern _MODULE MI_Cstring;
extern _MODULE MI_Ctypes;
extern _MODULE MI_Date;
extern _MODULE MM_DatePosix;
extern _MODULE MI_DragonInt;
extern _MODULE MM_DragonInt;
extern _MODULE MM_DragonT;
extern _MODULE MI_DragonT;
extern _MODULE MI_Env;
extern _MODULE MM_Env;
extern _MODULE MI_Extended;
extern _MODULE MM_Extended;
extern _MODULE MM_ExtendedFloat;
extern _MODULE MI_ExtendedFloat;
extern _MODULE MM_FPU;
extern _MODULE MI_FPU;
extern _MODULE MI_FS;
extern _MODULE MM_FSPosix;
extern _MODULE MI_File;
extern _MODULE MI_FilePosix;
extern _MODULE MM_FilePosix;
extern _MODULE MM_FileRd;
extern _MODULE MI_FileRd;
extern _MODULE MI_FileWr;
extern _MODULE MM_FileWr;
extern _MODULE MI_Fingerprint;
extern _MODULE MM_Fingerprint;
extern _MODULE MM_FloatMode;
extern _MODULE MI_FloatMode;
extern _MODULE MM_Fmt;
extern _MODULE MI_Fmt;
extern _MODULE MI_FmtBuf;
extern _MODULE MM_FmtBuf;
extern _MODULE MI_FmtBufF;
extern _MODULE MI_FmtBufTest;
extern _MODULE MI_IEEESpecial;
extern _MODULE MM_IEEESpecial;
extern _MODULE MM_Lex;
extern _MODULE MI_Lex;
extern _MODULE MM_LongFloat;
extern _MODULE MI_LongFloat;
extern _MODULE MI_LongReal;
extern _MODULE MM_LongReal;
extern _MODULE MI_LongRealRep;
extern _MODULE MI_M3_BUILTIN;
extern _MODULE MI_M3toC;
extern _MODULE MM_M3toC;
extern _MODULE MI_MachineID;
extern _MODULE MM_MachineIDPosix;
extern _MODULE MI_Main;
extern _MODULE MI_Math;
extern _MODULE MM_Math;
extern _MODULE MI_OSError;
extern _MODULE MM_OSErrorPosix;
extern _MODULE MI_OSErrorPosix;
extern _MODULE MI_Params;
extern _MODULE MM_Params;
extern _MODULE MM_ParseParams;
extern _MODULE MI_ParseParams;
extern _MODULE MI_Pathname;
extern _MODULE MM_PathnamePosix;
extern _MODULE MI_Pipe;
extern _MODULE MM_Pipe;
extern _MODULE MM_PipePosix;
extern _MODULE MM_Poly;
extern _MODULE MI_Poly;
extern _MODULE MI_PolyBasis;
extern _MODULE MM_PolyBasis;
extern _MODULE MI_Process;
extern _MODULE MM_ProcessPosix;
extern _MODULE MI_RT0;
extern _MODULE MM_RT0;
extern _MODULE MI_RT0u;
extern _MODULE MM_RT0u;
extern _MODULE MI_RTAllocator;
extern _MODULE MM_RTAllocator;
extern _MODULE MI_RTArgs;
extern _MODULE MM_RTArgs;
extern _MODULE MI_RTCollector;
extern _MODULE MM_RTCollector;
extern _MODULE MI_RTCollectorSRC;
extern _MODULE MI_RTExRep;
extern _MODULE MM_RTException;
extern _MODULE MI_RTException;
extern _MODULE MM_RTHeap;
extern _MODULE MI_RTHeap;
extern _MODULE MM_RTHeapDep;
extern _MODULE MI_RTHeapDep;
extern _MODULE MI_RTHeapEvent;
extern _MODULE MM_RTHeapInfo;
extern _MODULE MI_RTHeapInfo;
extern _MODULE MM_RTHeapMap;
extern _MODULE MI_RTHeapMap;
extern _MODULE MI_RTHeapRep;
extern _MODULE MM_RTHeapRep;
extern _MODULE MM_RTHooks;
extern _MODULE MI_RTHooks;
extern _MODULE MM_RTIO;
extern _MODULE MI_RTIO;
extern _MODULE MI_RTLinker;
extern _MODULE MM_RTLinker;
extern _MODULE MI_RTMachine;
extern _MODULE MM_RTMapOp;
extern _MODULE MI_RTMapOp;
extern _MODULE MI_RTMisc;
extern _MODULE MM_RTMisc;
extern _MODULE MM_RTModule;
extern _MODULE MI_RTModule;
extern _MODULE MI_RTOS;
extern _MODULE MM_RTOS;
extern _MODULE MI_RTParams;
extern _MODULE MM_RTParams;
extern _MODULE MI_RTPerfTool;
extern _MODULE MM_RTPerfTool;
extern _MODULE MM_RTProcedure;
extern _MODULE MI_RTProcedure;
extern _MODULE MI_RTProcedureSRC;
extern _MODULE MM_RTProcess;
extern _MODULE MI_RTProcess;
extern _MODULE MI_RTSignal;
extern _MODULE MM_RTSignal;
extern _MODULE MM_RTThread;
extern _MODULE MI_RTThread;
extern _MODULE MI_RTThreadInit;
extern _MODULE MM_RTThreadStk;
extern _MODULE MI_RTType;
extern _MODULE MM_RTType;
extern _MODULE MM_RTTypeMap;
extern _MODULE MI_RTTypeMap;
extern _MODULE MI_RTTypeSRC;
extern _MODULE MI_RTWeakRef;
extern _MODULE MM_Random;
extern _MODULE MI_Random;
extern _MODULE MI_RandomReal;
extern _MODULE MM_RandomReal;
extern _MODULE MI_Rd;
extern _MODULE MI_RdClass;
extern _MODULE MM_RdImpl;
extern _MODULE MM_RdMove;
extern _MODULE MI_Real;
extern _MODULE MM_Real;
extern _MODULE MM_RealFloat;
extern _MODULE MI_RealFloat;
extern _MODULE MI_RealRep;
extern _MODULE MI_RegularFile;
extern _MODULE MM_RegularFile;
extern _MODULE MM_Scan;
extern _MODULE MI_Scan;
extern _MODULE MI_Scheduler;
extern _MODULE MI_SchedulerPosix;
extern _MODULE MM_Stdio;
extern _MODULE MI_Stdio;
extern _MODULE MI_Swap;
extern _MODULE MM_Swap;
extern _MODULE MM_Terminal;
extern _MODULE MI_Terminal;
extern _MODULE MI_Text;
extern _MODULE MM_Text;
extern _MODULE MI_TextF;
extern _MODULE MM_TextRd;
extern _MODULE MI_TextRd;
extern _MODULE MM_TextSeq;
extern _MODULE MI_TextSeq;
extern _MODULE MI_TextSeqRep;
extern _MODULE MI_Thread;
extern _MODULE MI_ThreadEvent;
extern _MODULE MI_ThreadF;
extern _MODULE MM_ThreadPosix;
extern _MODULE MI_Tick;
extern _MODULE MM_TickPortable;
extern _MODULE MI_Time;
extern _MODULE MM_TimePosix;
extern _MODULE MI_TimePosix;
extern _MODULE MI_TimeStamp;
extern _MODULE MM_TimeStamp;
extern _MODULE MI_TimeStampRep;
extern _MODULE MM_Udir;
extern _MODULE MI_Udir;
extern _MODULE MM_Uerror;
extern _MODULE MI_Uerror;
extern _MODULE MI_Uexec;
extern _MODULE MI_Unetdb;
extern _MODULE MM_Unetdb;
extern _MODULE MM_Unix;
extern _MODULE MI_Unix;
extern _MODULE MM_UnsafeHash;
extern _MODULE MI_UnsafeRd;
extern _MODULE MI_UnsafeWr;
extern _MODULE MI_Uprocess;
extern _MODULE MI_Uresource;
extern _MODULE MM_Usignal;
extern _MODULE MI_Usignal;
extern _MODULE MI_Usocket;
extern _MODULE MI_Ustat;
extern _MODULE MI_Utime;
extern _MODULE MI_Utypes;
extern _MODULE MM_Utypes;
extern _MODULE MI_Uugid;
extern _MODULE MI_Uuio;
extern _MODULE MI_Word;
extern _MODULE MM_Word;
extern _MODULE MI_Wr;
extern _MODULE MI_WrClass;
extern _MODULE MM_WrMove;
extern _MODULE MM_WrPosix;
extern _MODULE MM_unibm;

static _MODULE *_modules[222] = {
  &MI_RTHeap,
  &MI_RTHeapInfo,
  &MI_Uprocess,
  &MI_RTSignal,
  &MI_RTAllocator,
  &MI_TimePosix,
  &MI_Time,
  &MI_ThreadEvent,
  &MI_Usignal,
  &MI_RTThread,
  &MI_Cerrno,
  &MI_RTThreadInit,
  &MI_SchedulerPosix,
  &MI_Scheduler,
  &MI_RTProcedureSRC,
  &MI_PolyBasis,
  &MI_Poly,
  &MI_Fingerprint,
  &MI_RTProcedure,
  &MI_RTExRep,
  &MI_RTException,
  &MI_RTProcess,
  &MI_Uresource,
  &MI_Uexec,
  &MI_RTPerfTool,
  &MI_RTArgs,
  &MI_RTParams,
  &MI_RTTypeMap,
  &MI_RTMapOp,
  &MI_RTHeapMap,
  &MI_RTHeapEvent,
  &MI_RTWeakRef,
  &MI_RTCollectorSRC,
  &MI_RTCollector,
  &MI_Uuio,
  &MI_Uerror,
  &MI_Utime,
  &MI_Utypes,
  &MI_Unix,
  &MI_RTOS,
  &MI_RTIO,
  &MI_Text,
  &MI_TextF,
  &MI_Cstddef,
  &MI_Cstdlib,
  &MI_M3toC,
  &MI_RTModule,
  &MI_RTTypeSRC,
  &MI_RTType,
  &MI_RT0u,
  &MI_Csetjmp,
  &MI_RTMachine,
  &MI_RTHeapDep,
  &MI_RT0,
  &MI_RTHeapRep,
  &MI_Word,
  &MI_BasicCtypes,
  &MI_Ctypes,
  &MI_Cstring,
  &MI_RTMisc,
  &MI_FloatMode,
  &MI_ThreadF,
  &MI_Thread,
  &MI_RTHooks,
  &MI_RTLinker,
  &MI_M3_BUILTIN,
  &MM_RTHeap,
  &MM_RTHeapInfo,
  &MM_RTSignal,
  &MM_RTLinker,
  &MM_RTAllocator,
  &MM_RTHooks,
  &MM_TimePosix,
  &MM_RTThread,
  &MM_RTThreadStk,
  &MM_Usignal,
  &MM_RTProcedure,
  &MM_PolyBasis,
  &MM_Poly,
  &MM_Fingerprint,
  &MM_RTException,
  &MM_RTProcess,
  &MM_RTPerfTool,
  &MM_RTArgs,
  &MM_RTParams,
  &MM_RTMapOp,
  &MM_RTTypeMap,
  &MM_RTHeapMap,
  &MM_RTCollector,
  &MM_Uerror,
  &MM_Unix,
  &MM_Utypes,
  &MM_RTOS,
  &MM_RTIO,
  &MM_Text,
  &MM_UnsafeHash,
  &MM_M3toC,
  &MM_RTModule,
  &MM_RTType,
  &MM_RT0u,
  &MM_RTHeapRep,
  &MM_RTHeapDep,
  &MM_RT0,
  &MM_Word,
  &MM_RTMisc,
  &MM_FloatMode,
  &MM_ThreadPosix,

  &MI_AtomAtomTbl,
  &MI_Atom,
  &MM_AtomAtomTbl,
  &MM_Atom,

  &MI_AtomList,
  &MM_AtomList,

  &MI_CConvert,
  &MM_CConvert,

  &MI_Convert,
  &MM_Convert,

  &MI_UnsafeWr,
  &MI_WrClass,
  &MI_Wr,
  &MM_WrMove,
  &MM_WrPosix,

  &MI_Real,
  &MM_Real,

  &MI_LongReal,
  &MM_LongReal,

  &MI_Extended,
  &MM_Extended,

  &MI_RealRep,

  &MI_DragonInt,
  &MM_DragonInt,

  &MI_DragonT,
  &MM_DragonT,

  &MI_FPU,
  &MM_FPU,

  &MI_RealFloat,
  &MM_RealFloat,

  &MI_LongRealRep,

  &MI_LongFloat,
  &MM_LongFloat,

  &MI_ExtendedFloat,
  &MM_ExtendedFloat,

  &MI_TextSeqRep,
  &MI_TextSeq,
  &MM_TextSeq,

  &MI_Pathname,
  &MM_PathnamePosix,

  &MI_Env,
  &MM_Env,

  &MI_Ustat,

  &MI_Udir,
  &MM_Udir,

  &MI_Uugid,

  &MI_Terminal,
  &MI_RegularFile,
  &MI_FS,
  &MI_Pipe,
  &MI_FilePosix,
  &MI_OSErrorPosix,
  &MI_OSError,
  &MI_File,
  &MI_Process,
  &MI_FmtBufTest,
  &MI_FmtBufF,
  &MI_FmtBuf,
  &MI_Fmt,
  &MM_Terminal,
  &MM_RegularFile,
  &MM_FSPosix,
  &MM_FilePosix,
  &MM_Pipe,
  &MM_PipePosix,
  &MM_ProcessPosix,
  &MM_OSErrorPosix,
  &MM_FmtBuf,
  &MM_Fmt,

  &MI_Math,
  &MM_Math,

  &MI_Tick,
  &MM_TickPortable,

  &MI_Date,
  &MM_DatePosix,

  &MI_Swap,
  &MM_Swap,

  &MI_TimeStampRep,

  &MI_Usocket,

  &MI_Unetdb,
  &MM_Unetdb,

  &MI_MachineID,
  &MM_MachineIDPosix,

  &MI_TimeStamp,
  &MM_TimeStamp,

  &MI_RandomReal,
  &MI_Random,
  &MM_RandomReal,
  &MM_Random,

  &MI_UnsafeRd,
  &MI_RdClass,
  &MI_Rd,
  &MM_RdImpl,
  &MM_RdMove,

  &MI_IEEESpecial,
  &MM_IEEESpecial,

  &MI_Lex,
  &MM_Lex,

  &MI_TextRd,
  &MM_TextRd,

  &MI_Scan,
  &MM_Scan,

  &MI_Params,
  &MM_Params,

  &MI_ParseParams,
  &MM_ParseParams,

  &MI_FileRd,
  &MM_FileRd,

  &MI_FileWr,
  &MM_FileWr,

  &MI_Stdio,
  &MM_Stdio,

  &MI_Main,
  &MM_unibm,

  0
};

static _LINK_INFO _m3_link_info = {
  /* n_modules  */ 221,
  /* modules    */ (_ADDRESS)_modules,
  /* argc       */ 0,
  /* argv       */ 0,
  /* envp       */ 0,
  /* instance   */ 0,
  /* stack_bot  */ 0,
  /* stack_top  */ (_ADDRESS)0x400000
};

int main (argc, argv, envp)
int argc;
char **argv;
char **envp;
{
  { /* initialize RTLinker's global data */
    _LINKER* linker = (_LINKER*)&MI_RTLinker;
    linker->info = &_m3_link_info;
    _m3_link_info.argc = argc;
    _m3_link_info.argv = (_ADDRESS)(argv);
    _m3_link_info.envp = (_ADDRESS)(envp);
    _m3_link_info.instance = (_ADDRESS)(0);
    _m3_link_info.bottom_of_stack = (_ADDRESS)(&linker);
  };

  /* finally, start the Modula-3 program */
  MM_RTLinker.main ();
  return 0;
}

