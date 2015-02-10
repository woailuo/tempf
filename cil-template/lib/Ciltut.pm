
# This package is used from an environment when CilConfig.pm has been loaded
package Ciltut;
use strict;

$::version_major = 1;
$::version_minor = 0;
$::version_sub = 0;

use App::Cilly;

# NOTE: If perl chokes, complaining about 'our', or
# "Array found where operator expected", it's because
# you need perl version 5.6.0 or later.
our @ISA = qw(App::Cilly);

sub new {
    my ($proto, @args) = @_;
    my $class = ref($proto) || $proto;

    # Select the directory containing Ciltut's executables.  We look in
    # both places in order to accomodate the build and distribution
    # directory layouts.
    my $bin;
    my $lib;
    if (-x "$::ciltuthome/obj/$::archos/ciltutcc.exe_opt") {
        $bin = "$::ciltuthome/obj/$::archos";
        $lib = "$::ciltuthome/obj/$::archos";
    } elsif (-x "$::ciltuthome/bin/ciltutcc.exe_opt") {
        $bin = "$::ciltuthome/bin";
        $lib = "$::ciltuthome/lib";
    } else {
        die "Couldn't find directory containing Ciltut executables.\n" .
            "Please ensure that Ciltut is compiled and installed properly.\n";
    }

    # Select the most recent executable
    my $mtime_asm = int((stat("$bin/ciltutcc.exe_opt"))[9]);
    my $mtime_byte = int((stat("$bin/ciltutcc.exe"))[9]);
    my $use_debug = 
            grep(/--bytecode/, @args) ||
            grep(/--ocamldebug/, @args) ||
            ($mtime_asm < $mtime_byte);
    if ($use_debug) { 
        $ENV{"OCAMLRUNPARAM"} = "b" . $ENV{"OCAMLRUNPARAM"}; # Print back trace
    } 

    # Save choice in global vars for printHelp (can be called from Cilly::new)
    $Ciltut::compiler = "$bin/ciltutcc" . ($use_debug ? ".exe" : ".exe_opt");

    my $self = Ciltut->App::Cilly::new(@args);

    # New variables for Ciltut
    $self->{COMPILER} = $Ciltut::compiler;
    $self->{LIBBASE} = $lib;

    $self->{DARWIN} = `uname` =~ /Darwin/;

    push @{$self->{CPP}}, $self->{INCARG} . $::ciltuthome . "/include";
    my $v = $::version_major * 1000 + $::version_minor * 100 + $::version_sub;
    push @{$self->{CPP}}, $self->{DEFARG} . (sprintf "__CILTUT__=%d", $v);

    # Override Cilly's default
    $self->{SEPARATE} = 1;

    $self->{TUT15} = 0;
    $self->{CONCOLICLIB} = "$lib/libconcolic_callbacks.$self->{LIBEXT}";

    bless $self, $class;
}

sub processArguments {
    my ($self) = @_;
    my @args = @{$self->{ARGV}};
    my $lib = $self->{LIBBASE};

    # Scan and process the arguments
    $self->setDefaultArguments;
    $self->collectArgumentList(@args);

    push @{$self->{CILTUTLIBS}}, "$lib/libciltut.$self->{LIBEXT}";

    return $self;
}

sub setDefaultArguments {
    my ($self) = @_;
    $self->{TRACE_COMMANDS} = 0;
    push @{$self->{CILARGS}}, "--home", $::ciltuthome;
    return $self->SUPER::setDefaultArguments;
}

sub collectOneArgument {
    my ($self, $arg, $pargs) = @_;
    my $res = 1;
    if ($self->compilerArgument($self->{OPTIONS}, $arg, $pargs)) {
        # do nothing
    } elsif ($arg eq "--help" || $arg eq "-help") {
        $self->printVersion();
        $self->printHelp();
        exit 0;
    } elsif ($arg eq "--version" || $arg eq "-version") {
        $self->printVersion();
        exit 0;
    } elsif ($arg eq "--trace") {
        $self->{TRACE_COMMANDS} = 1;
    } elsif ($arg eq "--bytecode") {
        $self->{NATIVECAML} = 0;
    } elsif ($arg =~ m|--save-temps=(.+)$|) {
        if (!-d $1) {
            die "Cannot find directory $1";
        }
        $self->{SAVE_TEMPS} = $1;
    } elsif ($arg eq '--save-temps') {
        $self->{SAVE_TEMPS} = '.';
    } elsif ($arg =~ m|--includedir=(.+)$|)  {
        push @{$self->{INCLUDEDIR}}, $1;
    } elsif ($arg =~ m|^--out=(\S+)$|) {
        # Intercept the --out argument
        $self->{CILLY_OUT} = $1;
        push @{$self->{CILARGS}}, "--out", $1;
    } elsif ($arg eq '--merge') {
        $self->{SEPARATE} = 0;
    } elsif ($arg eq '--enable-tut15') {
        $self->{TUT15} = 1;
        push @{$self->{CILARGS}}, "--enable-tut15";
    } elsif ($arg =~ m|^--|) {
        # All other arguments starting with -- are passed to CIL
        # Split the ==
        if ($arg =~ m|^(--\S+)=(.+)$|) {
            push @{$self->{CILARGS}}, $1, $2;
        } else {
            push @{$self->{CILARGS}}, $arg;
        }
    } else {
        # We fail!
        $res = 0;
    }
    return $res;
}

sub preprocess_before_cil {
    my($self, $src, $dest, $ppargs) = @_;
    my @args = @{$ppargs};
    unshift @args, $self->{INCARG} . $::ciltuthome . "/include";
    return $self->SUPER::preprocess_before_cil($src, $dest, \@args);
}


## We do not preprocess after CIL, to save time and files
sub preprocessAfterOutputFile {
    my ($self, $src) = @_;
    return $src; # Do not preprocess after CIL
}

sub preprocess_after_cil {
    my ($self, $src, $dest, $ppargs) = @_;
    if($src ne $dest) { die "I thought we are not preprocessing after CIL";}
    return $dest;
}

sub compile_cil {
    my ($self, $src, $dest, $ppargs, $ccargs) = @_;
    my @args = @{$ppargs};
    my @newargs;
    my $i;

    # Filter out -include options
    for ($i = 0; $i <= $#args; $i++) {
      $_ = $args[$i];
      if (/^-include/) {
        $i++;
      }
      else {
        push @newargs, $_;
      }
    }
    push @newargs, "$self->{INCARG}$::ciltuthome/include";
    return $self->SUPER::compile_cil($src, $dest, \@newargs, $ccargs);
}


sub link_after_cil {
    my ($self, $psrcs, $dest, $ppargs, $ccargs, $ldargs) = @_;
    my @srcs = @{$psrcs};
    my @libs = @{$ldargs};
    my @cargs = @{$ccargs};
    if ($self->{DARWIN}) {
        push @libs, "-Wl,-multiply_defined", "-Wl,suppress";
    }
    if (scalar @srcs == 0) {
        print STDERR "ciltutcc: no input files\n";
        return 0;
    } else {
        if ($self->{TUT15} == 1) {
            my $ocy = `ocamlfind query ocamlyices`;
            chomp($ocy);
            push @libs, "-L$::ciltuthome/lib", "-L/usr/lib/ocaml", "-L$ocy",
                        "-Wl,-rpath=$::ciltuthome/lib",
                        "-Wl,--start-group",
                        "-lcamlrun",
                        "-lciltut",
                        "-ltut15callbacks",
                        "-lunix",
                        "-lnums",
                        "-Wl,--end-group",
                        "-lcurses",
                        "-locamlyices",
                        "-lstdc++",
                        "-lm",
                        "/usr/local/lib/libyices.a";
        }
        else {
            push @libs, @{$self->{CILTUTLIBS}};
        }
        if ($self->{DARWIN}) {
          push @libs, "-ldl";
        }
        else {
          push @libs, "-ldl", "-lrt";
        }
        return $self->SUPER::link_after_cil(\@srcs, $dest, $ppargs,
                                            \@cargs, \@libs);
    }
}

sub linktolib {
    my ($self, $psrcs, $dest, $ppargs, $ccargs, $ldargs) = @_;
    my @srcs = @{$psrcs};
    my @libs = @{$ldargs};
    if (scalar @srcs == 0) {
        print STDERR "ciltutcc: no input files\n";
        return 0;
    } else {
        push @libs, @{$self->{CILTUTLIBS}};
        return $self->SUPER::linktolib(\@srcs, $dest, $ppargs,
                                       $ccargs, $ldargs);
    }
}

sub CillyCommand {
    my ($self, $ppsrc, $dest) = @_;

    my @cmd = ($self->{COMPILER});
    my $aftercil = $self->cilOutputFile($dest, 'cil.c');
    return ($aftercil, @cmd, '--out', $aftercil);
}

sub printVersion {
    printf "ciltutcc: %d.%d.%d\n", $::version_major, $::version_minor, $::version_sub;
}

sub printHelp {
    my ($self) = @_;
    my @cmd = ($self->{COMPILER}, '-help');
    print <<EOF;

Usage: ciltutcc [options] <source-files>...

Front end:

  --trace               Print commands invoked by the front end.
  --save-temps[=<dir>]  Save intermediate files (target directory optional).
  --version             Print version number and exit.
  --includedir          Add the specified directory to the beginning of
                        the include path.
  --gcc                 Use the specified executable instead of gcc as the
                        final C compiler (see also the --envmachine option)

EOF
    $self->runShell(@cmd); 
}

1;
