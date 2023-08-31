/*
* murphy/dom/dom_node.ixx
* 
* PURPOSE: 
* 
* CLASSES:
* 
* DESCRIPTION:
*	
* 
*/

export module murphy.dom.dom_node;

import <vector>;
import <string>;
import <unordered_map>;

import murphy.types;

export 
namespace murphy::dom
{
	// https://dom.spec.whatwg.org/#dom-node-nodetype
	enum class DOMNodeType : u16
	{
		Invalid = 0,
		Element = 1,
		Attribute = 2,
		Text = 3,
		CDataSection = 4,
		EntityReference = 5, // legacy
		EntityNode = 6, // legacy
		ProcessingInstruction = 7,
		Comment = 8,
		Document = 9,
		DocumentType = 10,
		DocumentFragment = 11,
	};


	enum class DocumentPosition : u16
	{
		Equal = 0,
		Disconnected = 1,
		Preceding = 2,
		Following = 4,
		Contains = 8,
		ContainedBy = 16,
		ImplementationSpecific = 32,
	};


	// https://dom.spec.whatwg.org/#node
	class Node
	{
	public:


		auto NodeName() const noexcept -> std::string_view
		{
			return name;
		}

		constexpr
		auto NodeType() const noexcept -> DOMNodeType
		{
			return type;
		}


		auto BaseURI() const -> std::string_view;

		auto IsConnected() const noexcept -> bool;

		auto OwnerDocument(); // -> Document;

		auto GetRootNode(/*options*/) -> non_owned_res<Node>;

		auto ParentNode() -> non_owned_res<Node>;
		auto ParentNode() const -> non_owned_res<const Node>;


	private:
		DOMNodeType type = DOMNodeType::Invalid;
		std::string name;

		non_owned_res<Node> parent;
		
		std::unordered_map<std::string, std::string> attributes;
		std::string content;
	};
} // namespace murphy::dom