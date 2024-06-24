# Changes log
The format of this file ins based in [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) and try to respect [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<br />

## [v0.1] - Release en curso

### Added
- Merge branch [doitegen_benckmark](https://github.com/MarioInf-UCM/TFM_u200_parallelComputation/pull/2/commits) - [@marioa25](https://github.com/marioa25)
    - Device kernels Op2 versions `ker_cholesky_Opt2`, `ker_doitgen_Opt2`, `ker_gemm_Opt2`, `ker_gemm_per_Opt2`, and `ker_jacobi2d_Opt2` developed and added.
    - Host classes `doitgenHost_0p0`, `doitgenHost_0p1`, `doitgenHost_0p2`, `choleskyHost_Op0`,`choleskyHost_Op1`,`choleskyHost_Op2`, `gemmHost_Op0`, `gemmHost_Op1`, `gemmHost_Op2` and `jacobi2dHost_Opt0`, `jacobi2dHost_Opt1`, `jacobi2dHost_Opt2` developed and added.


- Merge branch [doitegen_benckmark](https://github.com/MarioInf-UCM/TFM_u200_parallelComputation/pull/1/commits) - [@marioa25](https://github.com/marioa25)
    - Services `externProgramsConnection_service`, `fileWriter_service`, and `json_service` developed and added.
    - utilitie `dateAndTime` develoed and added.
    - utilities `evcen_timer`, `line_exception` and `xilinx_ocl_helper` added from [Get moving with Alveo](https://github.com/Xilinx/Get_Moving_With_Alveo).
    - Kernel classes `doitgenKernel`, `choleskyKernel`, `gemmKernel` and `jacobi_2dKernel` developed and added.
    - Host classes `doitgenHost`, `choleskyHost`, `gemmHost` and `jacobi_2dHost` developed and added.
    - Device kernels `ker_cholesky_noOpt`, `ker_cholesky_Opt`, `ker_doitgen_noOpt`, `ker_doitgen_Opt`, `ker_gemm_noOpt`, `ker_gemm_Opt`, `ker_gemm_per_noOpt`, `ker_gemm_per_Opt`, , `ker_jacobi_2d_noOpt` and `ker_jacobi_2d_Opt` developed and added.
    - Files `main` and `configParams` developed and added.
    - Python file `generatePyctures` developed and added.
    - Cmake file `CMakeList.txt` developed and added.
    - Cmake file `FindXRT.cmake` added from [Get moving with Alveo](https://github.com/Xilinx/Get_Moving_With_Alveo).
    - File `Makefile` developed and added.

### Fixed



### Changed
- Merge branch [doitegen_benckmark](https://github.com/MarioInf-UCM/TFM_u200_parallelComputation/pull/2/commits) - [@marioa25](https://github.com/marioa25)
    - Divided Opt host classes to versions Opt1 and Opt2.
    - Divided npOpt host classes to Opt0 version.
    - Divided Opt device's kernels to versions Opt1 and Opt2.
    - Divided Opt device's kernels to Opt0 version.





### Removed


[v0.1]: https://github.com/MarioInf-UCM/TFM_u200_parallelComputation/tree/release_v0.1