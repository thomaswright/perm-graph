[@bs.deriving abstract]
type node = {
  id: string,
  label: string,
};

[@bs.deriving abstract]
type edge = {
  id: string,
  label: string,
  source: string,
  target: string,
};

[@bs.deriving abstract]
type position2D = {
  x: int,
  y: int,
};

[@bs.deriving abstract]
type nodeData = {
  data: node,
  position: position2D,
};

[@bs.deriving abstract]
type edgeData = {data: edge};

[@bs.deriving abstract]
type elements = {
  nodes: array(nodeData),
  edges: array(edgeData),
};

let rec permutations = l => {
  let n = List.length(l);
  if (n == 1) {
    [l];
  } else {
    let rec sub = e =>
      fun
      | [] => failwith("sub")
      | [h, ...t] =>
        if (h == e) {
          t;
        } else {
          [h, ...sub(e, t)];
        };
    let rec aux = k => {
      let e = List.nth(l, k);
      let subperms = permutations(sub(e, l));
      let t = List.map(a => [e, ...a], subperms);
      if (k < n - 1) {
        List.rev_append(t, aux(k + 1));
      } else {
        t;
      };
    };
    aux(0);
  };
};

let nextPerm = p => {
  p
  |> List.mapi((i, e) => (i, e))
  |> List.sort(((_ai, ae), (_bi, be)) => ae - be)
  |> List.map(((i, _e)) => i + 1);
};

let findIndex = {
  let rec loop = (pred, xs, i) =>
    switch (xs) {
    | [] => None
    | [a, ...b] => pred(a) ? Some(i) : loop(pred, b, i + 1)
    };
  (pred, xs) => loop(pred, xs, 0);
};

let default = (d, o) =>
  switch (o) {
  | None => d
  | Some(a) => a
  };

let nextPermId = (l, p) =>
  l |> findIndex(a => a == nextPerm(p)) |> default(-1);

let permutationToString = p =>
  "["
  ++ List.fold_left((a, c) => a ++ string_of_int(c) ++ ",", "", p)
  ++ "]";

let data = permInts => {
  let myPerms = permutations(permInts);

  let topSpacing = 50;
  let ySpacing = 100;
  let leftSpacing = 50;
  let xSpacing = 75;

  let nodes: list(nodeData) =
    myPerms
    |> List.mapi((i, p) => {
         let nodeString = "node_" ++ string_of_int(i);
         let nextPermId = nextPermId(myPerms, p);

         let (x, y) =
           {
             nextPermId != i && nextPermId < i;
           }
             ? (nextPermId, ySpacing + topSpacing) : (i, topSpacing);

         (node(~id=nodeString, ~label=permutationToString(p)), x, y);
       })
    |> List.mapi((i, (node, x, y)) =>
         nodeData(
           ~data=node,
           ~position=position2D(~x=leftSpacing + x * xSpacing, ~y),
         )
       );

  let edges: list(edgeData) =
    myPerms
    |> List.mapi((i, p) => {
         let edgeString = "edge_" ++ string_of_int(i);
         edge(
           ~id=edgeString,
           ~label=edgeString,
           ~source="node_" ++ string_of_int(i),
           ~target=
             "node_"
             ++ {
               nextPermId(myPerms, p) |> string_of_int;
             },
         );
       })
    |> List.map(edge => edgeData(~data=edge));

  elements(
    ~nodes=Belt.List.toArray(nodes),
    ~edges=Belt.List.toArray(edges),
  );
};

module Cytoscape = {
  [@bs.module "./Cytoscape.js"] [@react.component]
  external make: (~elements: elements) => React.element = "default";
};

[@react.component]
let make = () => {
  <div
    style={ReactDOMRe.Style.make(
      ~borderStyle="solid",
      ~borderWidth="1px",
      (),
    )}>
    <Cytoscape elements={data([1, 2, 3, 4])} />
  </div>;
};