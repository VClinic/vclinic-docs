******************
API List
******************

======================
Vprofile
======================

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Initializing Vprofile
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The purpose of this step is to initialize Vprofile and register instrumentation functions to operate on the specified basic blocks and instructions:

-------------------
vprofile_init()
-------------------

.. code-block:: c++

   bool vprofile_init(bool (*filter)(instr_t *),
                      void* (*user_data_cb)(void *, instr_t *, instrlist_t *, opnd_t),
                      void (*ins_instrument_cb)(void *, instr_t *, instrlist_t *),
                      void (*bb_instrument_cb)(void *, instrlist_t *),
                      uint8_t flag)


- ``filter``: an instruction filter;

- ``user_data_cb``: a user_data delivery function allowing the user to return a tracable info pointer for the given opnd;

- ``ins_instrument_cb``: an instrumentation callback function for instructions;

- ``bb_instrument_cb``:an instrumentation callback function for basic blocks;

- ``flag``: a mode bitvector flag to tell Vprofile how to operate.

.. note::

   After the initialization, you could call the function ``vprofile_exit(void)`` before the client stops running to clear and free all the data used by Vprofile.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Register Trace
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

After you initialize Vprofile, the next step is to register tracer to trace values and information for analysis:

-------------------
vprofile_register_trace()
-------------------

.. code-block:: c++

   vtrace_t *vprofile_register_trace(bool (*filter)(opnd_t, vprofile_src_t),
                                     uint32_t opnd_mask,
                                     void (*update_cb)(val_info_t *),
                                     bool do_data_centric)


- ``filter``: an operand filter;

- ``opnd_mask``: a mask to filter operand, should be OR combined with values defined in @see `vprofile_src_t <https://vclinic.readthedocs.io/en/latest/api.html#vprofile-src-t>`_;

- ``update_cb``: an update function allowing user to update with the traced values when the buffer is full.;

- ``do_data_centric``: a flag to tell Vprofile tracer to trace values and information for data centric analysis.

.. note::

   This function may be equivilant with @see `vprofile_register_trace_ex <https://vclinic.readthedocs.io/en/latest/api.html#vprofile-register-trace-ex>`_, when:

- trace_addr=do_data_centric,

- trace_cct=!do_data_centric,

- trace_info=false,

- strictly_ordered=true,

- trace_reg_in_memref=false,

- trace_before_write=false.

We also provide an API to register trace that provides more adjustable options for trace:

--------------------------------------
vprofile_register_trace_ex()
--------------------------------------

.. code-block:: c++

   vtrace_t* vprofile_register_trace_ex(bool (*filter)(opnd_t, vprofile_src_t),
                                        uint32_t opnd_mask,
                                        void (*update_cb)(val_info_t *),
                                        uint32_t trace_flag)

- ``filter``: an operand filter;
- ``opnd_mask``: a mask to filter operand, should be OR combined with values defined in @see `vprofile_src_t <https://vclinic.readthedocs.io/en/latest/api.html#vprofile-src-t>`_;
- ``update_cb``: an update function allowing user to update with the traced values when the buffer is full.;
- ``trace_flag``: a flag to tell the registered trace user setting, including:

  - ``VPROFILE_TRACE_VALUE``: tell Vprofile tracer to trace value of a opnd.
  - ``VPROFILE_TRACE_ADDR``: tell Vprofile tracer to trace memory address/source register.
  - ``VPROFILE_TRACE_CCT``: tell Vprofile tracer to trace calling context.
  - ``VPROFILE_TRACE_INFO``: tell Vprofile tracer to trace additional info specified by the user.
  - ``VPROFILE_TRACE_STRICTLY_ORDERED``: tell Vprofile to trace the value in a single trace buffer and trace the data type of each access.
  - ``VPROFILE_TRACE_REG_IN_MEMREF``: tell Vprofile tracer to trace the register values used in memory operand.
  - ``VPROFILE_TRACE_BEFORE_WRITE``: tell Vprofile tracer to trace the register/memory values before overwritten (thus vprofile_src_t marked as WRITE|BEFORE).
  
--------------------------------------
vprofile_allocate_trace()
--------------------------------------

.. code-block:: c++

   vtrace_t* vprofile_allocate_trace(uint32_t trace_flag)
   
Allocate a new vprofile trace data structure with trace_flag.

.. note::

   the trace buffer is not registered (thus NULL)

--------------------------------------
vprofile_register_trace_cb()
--------------------------------------

.. code-block:: c++

   void vprofile_register_trace_cb(vtrace_t *vtrace, 
                                   bool (*filter)(opnd_t, vprofile_src_t),
                                   uint32_t opnd_mask, 
                                   vprofile_data_t data_type,
                                   void (*update_cb)(val_info_t *))
   
Register update callbacks for the specified data_type within the given vtrace.

.. note::

   If the vtrace is configured as strictly_ordered, the data_type must be ANY; otherwise it will result in assertion failure (usage error).
   If the data_type is set as ANY, all trace buffer is registered with the given callbacks.
   
--------------------------------------
vprofile_register_trace_template_cb()
--------------------------------------

.. code-block:: c++

   #define vprofile_register_trace_template_cb(vtrace, filter, opnd_mask, update_cb)
   
A convenient macro function that registers update callbacks for all supported data types.

.. note::

   Supported data types @see `vprofile_data_t <https://vclinic.readthedocs.io/en/latest/api.html#vprofile-data-t>`_.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Enum
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

--------------------------------------
vprofile modes
--------------------------------------

.. code-block:: c++

   enum {
      VPROFILE_DEFAULT = 0x00,
      VPROFILE_COLLECT_CCT = 0x01,
      VPROFILE_COLLECT_DATAOBJ = 0x02,
      VPROFILE_COLLECT_DATAOBJ_TREE_BASED = 0x04,
      VPROFILE_COLLECT_DATAOBJ_ADDR_RANGE = 0x08,
   };

- ``VPROFILE_DEFAULT``: Tell VClinic to only keep track of general information such as values, addresses, etc. and no need to use DrCCTProf to collect calling contexts;
- ``VPROFILE_COLLECT_CCT``: Tell VClinic to use DrCCTProf to collect calling contexts;
- ``VPROFILE_COLLECT_DATAOBJ``: Tell VClinic to use DrCCTProf to collect data-centric message;
- ``VPROFILE_COLLECT_DATAOBJ_TREE_BASED``: Not yet implemented;
- ``VPROFILE_COLLECT_DATAOBJ_ADDR_RANGE``: Not yet implemented.

--------------------------------------
vprofile_data_t
--------------------------------------

.. code-block:: c++

   enum vprofile_data_t {
      ANY = -1,
      INT8 = 0,
      INT16,
      INT32,
      SPx1,
      INT64,
      DPx1,
      INT128,
      INT8x16,
      SPx4,
      DPx2,
      INT256,
      INT8x32,
      SPx8,
      DPx4,
      INT512,
      INT8x64,
      SPx16,
      DPx8,
      NUM_DATA_TYPES
   };

Data types currently supported for tracing.

--------------------------------------
vprofile_src_t
--------------------------------------

.. code-block:: c++

   enum vprofile_src_t {
      VPROFILE_SRC_INVALID=0x0,
      // basic opnd attributes
      GPR_REGISTER=0x1,
      SIMD_REGISTER=0x2,
      CTR_REGISTER=0x4,
      OTH_REGISTER=0x8,
      PC=0x10,
      MEMORY=0x20,
      IMMEDIATE=0x40,
      // basic action attributes
      READ=0x100,
      WRITE=0x200,
      // basic value profiling position attributes
      // this is annotation of where the value trace point come from
      // BEFORE is before the instruction execution, and
      // AFTER is after the instruction execution.
      BEFORE=0x1000,
      AFTER=0x2000,
      // is floating
      IS_INTEGER=0x4000,
      IS_FLOATING=0x8000,
      ANY_DATA_TYPE=(IS_INTEGER|IS_FLOATING),
      // derived attributes
      REGISTER=(GPR_REGISTER|SIMD_REGISTER|CTR_REGISTER|OTH_REGISTER),
      REGISTER_READ=(REGISTER|READ|BEFORE),
      REGISTER_WRITE=(REGISTER|WRITE|AFTER),
      REGISTER_BEFORE_WRITE=(REGISTER|WRITE|BEFORE),
      PC_READ=(PC|READ|BEFORE),
      PC_WRITE=(PC|WRITE|AFTER),
      PC_BEFORE_WRITE=(PC|WRITE|BEFORE),
      MEMORY_READ=(MEMORY|READ|BEFORE),
      MEMORY_WRITE=(MEMORY|WRITE|BEFORE),
      MEMORY_BEFORE_WRITE=(REGISTER|WRITE|BEFORE)
   };

Attributes used to describe operands. The targeted operand can be customized by combining different attributes.

--------------------------------------
Trace Flags
--------------------------------------

.. code-block:: c++

   enum {
      VPROFILE_TRACE_INVALID=0x0,
      VPROFILE_TRACE_VALUE=0x1,
      VPROFILE_TRACE_ADDR=0x2,
      VPROFILE_TRACE_CCT=0x4,
      VPROFILE_TRACE_INFO=0x8,
      VPROFILE_TRACE_STRICTLY_ORDERED=0x10,
      VPROFILE_TRACE_REG_IN_MEMREF=0x20,
      VPROFILE_TRACE_BEFORE_WRITE=0x40,
      // DEFAULT
      VPROFILE_TRACE_DEFAULT=VPROFILE_TRACE_VALUE,
      // valid combinations
      VPROFILE_TRACE_VAL_CCT_ADDR_INFO = 
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_CCT | VPROFILE_TRACE_ADDR | VPROFILE_TRACE_INFO,
      VPROFILE_TRACE_VAL_CCT_ADDR = 
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_CCT | VPROFILE_TRACE_ADDR,
      VPROFILE_TRACE_VAL_CCT_INFO = 
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_CCT | VPROFILE_TRACE_INFO,
      VPROFILE_TRACE_VAL_CCT =
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_CCT,
      VPROFILE_TRACE_VAL_ADDR_INFO = 
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_ADDR | VPROFILE_TRACE_INFO,
      VPROFILE_TRACE_VAL_ADDR = 
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_ADDR,
      VPROFILE_TRACE_VAL_INFO = 
         VPROFILE_TRACE_VALUE | VPROFILE_TRACE_INFO,
      VPROFILE_TRACE_CCT_ADDR_INFO = 
         VPROFILE_TRACE_CCT | VPROFILE_TRACE_ADDR | VPROFILE_TRACE_INFO,
      VPROFILE_TRACE_CCT_ADDR = 
         VPROFILE_TRACE_CCT | VPROFILE_TRACE_ADDR,
      VPROFILE_TRACE_CCT_INFO = 
         VPROFILE_TRACE_CCT | VPROFILE_TRACE_INFO,
      VPROFILE_TRACE_ADDR_INFO = 
         VPROFILE_TRACE_ADDR | VPROFILE_TRACE_INFO
   };

Used to specify information to be traced for a target operand. For example, for VPROFILE_TRACE_VAL_ADDR, each unit in the buffer will contain the value and address information of a target operand.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Data Structures
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-------------------
vtrace_t
-------------------

.. code-block:: c++

   struct vtrace_t {
      uint32_t trace_flag;
      uint32_t opnd_mask;
      union {
         /* strictly_ordered, so only single buffer with extra tracing of data types
         * is allocated */
         vtrace_buffer_t *buff;
         /* not strictly ordered, so each data type maintains its own buffer */
         vtrace_buffer_t *buff_ex[NUM_DATA_TYPES];
      };
   };

Data structure to store information of a registered trace.

- ``trace_flag``: Combination of ``Trace Flags``, which used to specify information to be traced for a target operand;

- ``opnd_mask``: Combination of ``vprofile_src_t``, which used to filter operands to be traced;

- ``buff/buff_ex``: trace buffer, @see `vtrace_buffer_t <https://vclinic.readthedocs.io/en/latest/api.html#vtrace-buffer-t>`_.

-------------------
val_info_t
-------------------

.. code-block:: c++

   struct val_info_t {
      uint64_t addr;
      // vprofile_src_t
      uint32_t type;
      int32_t ctxt_hndl;
      void *val;
      void *info;
      uint8_t size;
      uint8_t esize;
      bool is_float;
   };

Data structure to pass the traced values into trace updating callbacks. Note that some of the values may not be filled in to avoid unnecessary message passing when the information can be assumed to be already known for the caller (e.g., size/esize when not strictly_ordered, no addr info, etc).

.. note::

   if type & GPR_REGISTER, addr is gpr register id;

   else if type & SIMD_REGISTER, addr is simd register id;

   else if type & CTR_REGISTER, addr is control register id;

   else if type & OTH_REGISTER, addr is dynamorio register id;

   else if type & MEMORY, addr is a memory address;

   else: undefined

======================
Vtracer
======================

.. attention::

   In most cases, users only need to interact with ``Vtracer`` through the ``Vprofile`` interface, instead of calling the ``Vtracer`` interface directly.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Initializing Vtracer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The purpose of this step is to initialize Vtracer:

-------------------
vtracer_init()
-------------------

.. code-block:: c++

   bool vtracer_init(void)

.. note::

   Call the function ``vtracer_exit(void)`` to clear and free all the resourced allocated by VTracer.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Creating trace buffer
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-------------------
vtracer_create_trace_buffer()
-------------------

.. code-block:: c++

   vtrace_buffer_t *vtracer_create_trace_buffer(uint buffer_size)

- ``buffer_size``: buffer size in bytes for trace buffer;

.. note::

   This routine will create trace buffer with NULL callbacks registered: 
   @see `vtracer_create_trace_buffer_ex <https://vclinic.readthedocs.io/en/latest/api.html#vtracer-create-trace-buffer-ex>`_

-------------------
vtracer_create_trace_buffer_ex()
-------------------

.. code-block:: c++

   vtrace_buffer_t *
   vtracer_create_trace_buffer_ex(uint buffer_size,
                                  vtracer_buf_full_cb_t full_cb,
                                  void* user_data_full,
                                  vtracer_buf_fill_num_cb_t fill_num_cb,
                                  void *user_data_fill_num)

- ``buffer_size``: buffer size in bytes for trace buffer;

- ``full_cb``: the callback to register which will be called to update with the buffered trace when the trace buffer is detected as full;

- ``user_data_full``: the user-specified data for the full_cb callback. This registered user_data will send as the user_data parameter of full_cb callback;

- ``fill_num_cb``: the callback to register which will be called during analysis phase to obtain how many slots (elements) will be filled in for the given instruction;

- ``user_data_fill_num``: the user-specified data for the fill_num_cb callbacks. This registered user_data will send as the user_data parameter of fill_num_cb callback.

.. important::
   With a given non-NULL callbacks, VTracer will guarantee that the full_cb will be called once the trace buffer is detected as full, and fill_num_cb is used to estimate the filled in slots for detecting potential trace buffer overflow. 
   When the VTracer detects any inconsistancy of estimated filled in number given by fill_num_cb and actual filled in slots (expensive check only in DEBUG mode), it will assert for a usage error with sufficient information for further debugging.

.. note::
   Call the function ``vtracer_buffer_free(vtrace_buffer_t *buf)`` to free all resources allocated for this vtrace buffer: @param buf.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Sampling
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-------------------
vtracer_enable_sampling()
-------------------

.. code-block:: c++

   void vtracer_enable_sampling(int window_enable,
                                int window_disable)

- ``window_enable``: buffer size in bytes for trace buffer;

- ``window_disable``: non-sample window size to disable value tracing & updating.

.. hint::

   The sample rate can be calculated as: 
      window_enable / window_disable

.. note::
   The sampling window is applied in the number of executed instructions.

-------------------
vtracer_disable_sampling()
-------------------

.. code-block:: c++

   void vtracer_disable_sampling()

Disable sampling.

-------------------
vtracer_get_sampling_state()
-------------------

.. code-block:: c++

   bool vtracer_get_sampling_state(void *drcontext)

Return true if sampling enabled; otherwise return false.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Tracing
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-------------------
vtracer_get_trace_buffer_in_reg()
-------------------

.. code-block:: c++

   void vtracer_get_trace_buffer_in_reg(void *drcontext, 
                                        instr_t *where,
                                        instrlist_t *ilist,
                                        vtrace_buffer_t *vtrace_buffer,
                                        reg_id_t reg_ptr)

Get the current trace buffer pointer in the given register.

- ``vtrace_buffer``: the vtrace buffer to query the pointer value;

- ``reg_ptr``: the target register to store the current trace buffer of vtrace_buffer. The register should be reserved by the caller.

.. note::

   The instrumentation will be inserted before @param where in @param ilist, same below.

-------------------
vtracer_insert_trace_forward()
-------------------

.. code-block:: c++

   void vtracer_insert_trace_forward(void *drcontext, 
                                     instr_t *where,
                                     instrlist_t *ilist, 
                                     int size,
                                     vtrace_buffer_t *vtrace_buffer,
                                     reg_id_t reg_ptr, 
                                     reg_id_t scratch)

Move the trace buffer pointer forward and store back to the vtrace_buffer if valid.

- ``size``: the size in bytes to move forward the trace buffer pointer in reg_ptr;

- ``vtrace_buffer``: the vtrace buffer to store back the forwarded pointer. May be NULL when writeback to the vtrace buffer is not expected. In that case, VTracer will not insert instrumetations to update trace buffer pointers;

- ``reg_ptr``: the register to hold the value of current buffer pointer. It will hold new forwarded buffer pointer if vtrace_buffer is not NULL; otherwise its return value is undefined;

- ``scratch``: free scratch register given by caller. its return value is undefined.

-------------------
vtracer_insert_trace_val()
-------------------

.. code-block:: c++

   void vtracer_insert_trace_val(void *drcontext, 
                                 instr_t *where,
                                 instrlist_t *ilist, 
                                 opnd_t ref, 
                                 reg_id_t reg_ptr,
                                 reg_id_t scratch, 
                                 ushort offset)

Insert instrumentations to store the value of ref into vtrace_buffer.

- ``ref``: can be register/memory/immediate operand;

- ``reg_ptr``: the register to hold the value of current buffer pointer. Its value will not be modified;

- ``scratch``: free scratch register given by caller. its return value is undefined;

- ``offset``: offsets to the target domain of a value trace slot.

.. hint::

   In general, this function will insert codes equivilant to: 
      <reg_ptr>.<offset> = <ref>

-------------------
vtracer_insert_trace_constant()
-------------------

.. code-block:: c++

   void vtracer_insert_trace_constant(void *drcontext, 
                                      instr_t *where,
                                      instrlist_t *ilist, 
                                      T val, 
                                      reg_id_t reg_ptr,
                                      reg_id_t scratch, 
                                      ushort offset)

Insert instrumentations to store the constant value of type T into vtrace_buffer.

- ``T``: the constant value's data type. Only size of 1,2,4,8,16,32,(64 for AVX512) is supported; otherwise it will result in static assertion failure at compile time;

- ``val``: the source constant value for tracing;

- ``reg_ptr``: the register to hold the value of current buffer pointer. Its value will not be modified;

- ``scratch``: free scratch register given by caller. its return value is undefined;

- ``offset``: offsets to the target domain of a value trace slot.

.. hint::

   In general, this function will insert codes equivilant to:  
      <reg_ptr>.<offset> = <val>


-------------------
vtracer_update_clean_call()
-------------------

.. code-block:: c++

   void vtracer_update_clean_call(void *drcontext, vtrace_buffer_t *buf, T cache)

Can be called by clean call func to trace T into buffer.

- ``T``: passed in struct type;

- ``buf``: the vtrace buffer to be filled in;

- ``cache``: passed in struct, will be filled into buffer.

^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Data Structure
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

-------------------
vtrace_buffer_t
-------------------

.. code-block:: c++

   typedef struct {
      uint buf_size;
      uint vec_idx;

      /* callbacks for buffer checking and updating */
      vtracer_buf_full_cb_t full_cb;
      void* user_data_full;
      vtracer_buf_fill_num_cb_t fill_num_cb;
      void* user_data_fill_num;

      /* tls implementation */
      int tls_idx;
      uint tls_offs;
      reg_id_t tls_seg;
   } vtrace_buffer_t;

Information about a trace buffer.

- ``buf_size``: buffer size in bytes;

- ``vec_idx``: index into the clients vector;

- ``full_cb``: callbacks for buffer updating. typedef void (\*vtracer_buf_full_cb_t)(void\* buf_base, void\* buf_end, void\* user_data);

- ``user_data_full``: ``user_data`` for ``full_cb``;

- ``fill_num_cb``: callbacks for buffer checking. typedef size_t (\*vtracer_buf_fill_num_cb_t)(void\* drcontext, instr_t\* where, void\* user_data);

- ``user_data_fill_num``: ``user_data`` for ``fill_num_cb``;