# [Krafton Jungle Week05] RB Tree κ΅¬ν

<br>

- π μ§ν κΈ°κ° : 2022. 11. 24. ~ 2022. 11. 30.
- π κ³Όμ  μ€λͺ : [GUIDELINES.md](./GUIDELINES.md)
- π­ κ°λ° μΌμ§ : [[Krafton Jungle | TIL_22.11.28 - 30] RB Tree κ΅¬ν](https://olive-su.tistory.com/424)
- π ν¨μ μ€λͺ κ΄λ ¨ λνλ¨ΌνΈ : [Jungle olive-su | RB Tree-Lab](https://jungle-olivesu.netlify.app/rbtree-lab/html/rbtree_8c.html) Β© doxygen
- π μ°Έκ³  μλ£ : [Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein - Introduction to Algorithms-The MIT Press](https://mitpress.mit.edu/9780262533058/introduction-to-algorithms/)

<br>

---

### π TEST RESULT

- 'SENTINEL' λ°©μμΌλ‘ κ΅¬ν

<br>

- input

```bash
>>> make test
```

<br>

- output

```bash
>>> cc -I ../src -Wall -g -DSENTINEL   -c -o test-rbtree.o test-rbtree.c
>>> cc   test-rbtree.o ../src/rbtree.o   -o test-rbtree
>>> ./test-rbtree
>>> Passed all tests!
>>> valgrind ./test-rbtree
>>> ==123343== Memcheck, a memory error detector
>>> ==123343== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
>>> ==123343== Using Valgrind-3.15.0 and LibVEX; rerun with -h for copyright info
>>> ==123343== Command: ./test-rbtree
>>> ==123343==
>>> Passed all tests!
>>> ==123343==
>>> ==123343== HEAP SUMMARY:
>>> ==123343==     in use at exit: 0 bytes in 0 blocks
>>> ==123343==   total heap usage: 20,123 allocs, 20,123 frees, 684,740 bytes allocated
>>> ==123343==
>>> ==123343== All heap blocks were freed -- no leaks are possible
>>> ==123343==
>>> ==123343== For lists of detected and suppressed errors, rerun with: -s
>>> ==123343== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

<br>

<br>

|[<img src="https://user-images.githubusercontent.com/67156494/210130075-978909cc-0eb7-45d0-8f74-b87d721abf58.png" width=120>](https://github.com/Krafton-Jungle-W05-Team01/Team01-rbtree-lab)
|-----|
| [Week05 Team 1 Repository](https://github.com/Krafton-Jungle-W05-Team01/Team01-rbtree-lab) |
