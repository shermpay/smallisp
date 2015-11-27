((c++-mode . ((clang-format-style . "Google")
              (eval .
                    (setq my-project-include-path
                          (concat "-I" (projectile-project-root) "include")))
              (eval
               . (setq flycheck-clang-args
                       (split-string
                        (shell-command-to-string
                         (concat *my-llvm-config* *my-llvm-config-args*)))))
              (eval . (add-to-list 'flycheck-clang-args
                                   my-project-include-path))
              (flycheck-clang-language-standard . "c++14")
              (eval . (google-set-c-style))
              (eval . (setq company-clang-arguments 
                            (list my-project-include-path)))
              (eval . (helm-gtags-mode))))
 (c-mode . ((mode . c++))))
