# [Krafton Jungle Week05] RB Tree 구현

<br>

- 📅 진행 기간 : 2022. 11. 24. ~ 2022. 11. 30.
- 📃 과제 설명 : [GUIDELINES.md](./GUIDELINES.md)
- 📭 개발 일지 : [[Krafton Jungle | TIL_22.11.28 - 30] RB Tree 구현](https://olive-su.tistory.com/424)
- 🗂 함수 설명 관련 도큐먼트 : [Jungle olive-su | RB Tree-Lab](https://jungle-olivesu.netlify.app/rbtree-lab/html/rbtree_8c.html) © doxygen
- 📖 참고 자료 : [Thomas H. Cormen, Charles E. Leiserson, Ronald L. Rivest, Clifford Stein - Introduction to Algorithms-The MIT Press](https://mitpress.mit.edu/9780262533058/introduction-to-algorithms/)

<br>

---

### 🎉 TEST RESULT

- 'SENTINEL' 방식으로 구현

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
