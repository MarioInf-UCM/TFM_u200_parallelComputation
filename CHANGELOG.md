# Registro de cambios

The format of this file ins based in [Keep a Changelog](https://keepachangelog.com/en/1.0.0/) and try to respect [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

<br />

## [v0.1] - Release en curso

### Added

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



### Removed


[v0.1]: https://github.com/MarioInf-UCM/TFM_u200_parallelComputation/tree/release_v0.1