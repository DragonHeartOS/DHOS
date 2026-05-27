{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
        nativeBuildInputs = [ ];
        buildInputs = with pkgs; [
          cmake
          ninja
        ];
      in
      {
        devShells.default =
          pkgs.mkShell {
            packages =
              with pkgs;
              [
                llvmPackages_21.clang-tools
                lldb
                codespell
              ]
              ++ buildInputs
              ++ nativeBuildInputs;
          };
      }
    );
}
