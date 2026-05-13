# Using mkShell from nixpkgs
{
  pkgs ? import <nixpkgs> { },
  ...
}:
pkgs.mkShell {
  name = "tools";
  packages = with pkgs; [
    # keep-sorted start

    avrdude
    bashInteractive
    nix-fast-build
    nix-output-monitor
    nix-tree
    platformio
    teensy-loader-cli
    tree
    # keep-sorted end
  ];
  shellHook = ''
    export PLATFORMIO_CORE_DIR=$PWD/.platformio
  '';

}

# https://github.com/numtide/blueprint/blob/main/docs/content/guides/configuring_direnv.md
