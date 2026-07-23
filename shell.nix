let
  pins = import ./pins.nix;
in
{
  pkgs ? import (builtins.fetchTarball pins.nixpkgs) { },
}:

pkgs.mkShell {
  packages = with pkgs; [
    editorconfig-checker
    llvmPackages_22.clang-tools
    nixfmt
    pre-commit
    python3Packages.identify

    dosbox-x
    open-watcom-bin
  ];

  shellHook = ''
    pre-commit install
  '';
}
