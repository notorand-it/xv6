{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { nixpkgs, ... }:
    let
      system = "x86_64-linux";

      riscv-pkgs = import nixpkgs {
        inherit system;

        crossSystem = {
          config = "riscv64-unknown-linux-gnu";
        };
      };

      pkgs = import nixpkgs {
        inherit system;
      };
    in
    {
      devShells.${system}.default = riscv-pkgs.mkShell {
        packages = with riscv-pkgs; [
          libgcc
        ] ++ (with pkgs; [
          gnumake
          qemu
          bear
        ]);
      };
    };
}
