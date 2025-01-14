open Oni_Model;
open Oni_IntegrationTestLib;

runTest(
  ~name="InsertMode test - effects batched to runEffects",
  (dispatch, wait, runEffects) => {
  wait(~name="Initial mode is normal", (state: State.t) =>
    state.mode == Vim.Types.Normal
  );

  dispatch(KeyboardInput("i"));

  wait(~name="Mode switches to insert", (state: State.t) =>
    state.mode == Vim.Types.Insert
  );

  /* Simulate multiple events getting dispatched before running effects */

  dispatch(KeyboardInput("A"));
  dispatch(KeyboardInput("B"));
  dispatch(KeyboardInput("C"));

  runEffects();

  wait(~name="Buffer shows ABC", (state: State.t) =>
    switch (Selectors.getActiveBuffer(state)) {
    | None => false
    | Some(buf) =>
      let line = Buffer.getLine(buf, 0);
      Log.info("Current line is: |" ++ line ++ "|");
      String.equal(line, "ABC");
    }
  );
});
