******************
QuickStart
******************

======================
Dependencies
======================

In order to build you'll need the following packages:

-   gcc (at least version 4.8)
-   binutils (at least version 2.26)
-   `cmake <https://cmake.org/download/>`_ (at least version 3.7)
-   perl

.. tip::

   VClinic leverages `DynamoRIO <https://github.com/DynamoRIO/dynamorio>`_ and `DrCCTProf <https://github.com/Xuhpclab/DrCCTProf>`_ to analyze a program execution and obtain full calling contexts on the fly.

======================
Build
======================

Get source code from the github repository and build VClinic:

.. code-block:: console

   $ git clone --recurse xxx
   $ ./build.sh

======================
Usage
======================

To run VClinic, one needs to use the following command:

-   **x86_64**

.. code-block:: console

   $ build/bin64/drrun -t <client tool> -- <application> [apllication args]

-   **aarch64**

.. code-block:: console

   $ build/bin64/drrun -unsafe_build_ldstex -t <client tool> -- <application> [apllication args]

======================
VClinic Tools
======================

VClinic provides different profiling tools. Currently the supported tools are:

-   vprofile_all_opnd
-   vprofile_mem_and_reg
-   vprofile_mem_and_reg_read
-   vprofile_memory
-   vprofile_memory_read
-   deadspy
-   redspy
-   loadspy
-   zerospy