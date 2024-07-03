function benchmark(LOOP_N, TEST_COUNT) {
  for (let i = 0; i < TEST_COUNT; i++) {
    let sum = 0;
    for (let j = 0; j < LOOP_N; j++) {
      sum += Math.random();
    }
  }
}
