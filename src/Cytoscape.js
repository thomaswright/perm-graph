import React from "react";
import ReactDOM from "react-dom";
import CytoscapeComponent from "react-cytoscapejs";

export default class MyApp extends React.Component {
  constructor(props) {
    super(props);
  }

  render() {
    // const elements = [
    //   { data: { id: "one", label: "Node 1" }, position: { x: 0, y: 0 } },
    //   { data: { id: "two", label: "Node 2" }, position: { x: 100, y: 0 } },
    //   {
    //     data: {
    //       source: "one",
    //       target: "two",
    //       label: "Edge from Node1 to Node2"
    //     }
    //   }
    // ];

    console.log(this.props.elements);

    return (
      <CytoscapeComponent
        elements={CytoscapeComponent.normalizeElements(this.props.elements)}
        style={{ width: "100%", height: "100%" }}
      />
    );
  }
}
