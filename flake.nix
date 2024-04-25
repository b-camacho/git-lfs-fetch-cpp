{
  description = "C++ library for fetching git-lfs files";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }: let
    pkgs = import nixpkgs {};
  in {
    defaultPackage.x86_64-linux = pkgs.stdenv.mkDerivation {
      name = "git-lfs-fetch";
      src = self;
      buildInputs = [ pkgs.clang pkgs.gnumake pkgs.curl pkgs.nlohmann_json ];
      buildPhase = ''
        make
      '';
      installPhase = ''
        mkdir -p $out/lib
        cp git-lfs-fetch.a $out/lib/
      '';
      checkPhase = ''
        make test
      '';
    };
  };
}

