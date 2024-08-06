{
  description = "C++ library for fetching git-lfs files";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  outputs = { self, nixpkgs }:
    let
      inherit (self) outputs;
      systems =
        [ "aarch64-linux" "x86_64-linux" "aarch64-darwin" "x86_64-darwin" ];
      forAllSystems = nixpkgs.lib.genAttrs systems;
    in {
      packages = forAllSystems (system:
        let pkgs = import nixpkgs { inherit system; };
        in {
          default = pkgs.stdenv.mkDerivation {
            name = "git-lfs-fetch";
            src = self;
            buildInputs = [
              pkgs.clang
              pkgs.gnumake
              pkgs.curl
              pkgs.nlohmann_json
              pkgs.libgit2
            ];
            buildPhase = ''
              make
            '';
            installPhase = ''
              mkdir -p $out/lib
              cp *.a $out/lib/
            '';
            checkPhase = ''
              make test
            '';
          };
        });
    };

}

