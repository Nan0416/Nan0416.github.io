Created: 2020-04-24
Modified: 2020-04-24

* [Introduction](#intro)
* [References](#reference)
***
### <a id="intro">Introduction</a>
A XML file is a data container, where you can define your own tags. For example
```xml
<?xml version="1.0" encoding="UTF-8"?>
<info>
    <firstname>Nan</firstname>
    <lastname>Qin</lastname>
</info>
```

#### XML Scheme
XML Scheme defines a format for a XML. In general, you can have any tags in your XML, but XML scheme will restrict the valid tags and content.

The XML Scheme itself is defined as a XML file, and saved as .xsd file. Below is an spring bean xsd example.
```xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>

<xsd:schema xmlns="http://www.springframework.org/schema/beans"
		xmlns:xsd="http://www.w3.org/2001/XMLSchema"
		targetNamespace="http://www.springframework.org/schema/beans">

<!-- top element-->
<xsd:element name="beans">
    <xsd:complexType>
        <xsd:sequence>
			<xsd:element ref="description" minOccurs="0"/>
			<xsd:choice minOccurs="0" maxOccurs="unbounded">
				<xsd:element ref="import"/>
				<xsd:element ref="alias"/>
				<xsd:element ref="bean"/>
				<xsd:any namespace="##other" processContents="strict" minOccurs="0" maxOccurs="unbounded"/>
			</xsd:choice>
			<xsd:element ref="beans" minOccurs="0" maxOccurs="unbounded"/>
		</xsd:sequence>
        <xsd:attribute name="profile" use="optional" type="xsd:string"></xsd:attribute>
        <xsd:attribute name="default-lazy-init" default="default" type="defaultable-boolean"></xsd:attribute>
		<xsd:attribute name="default-merge" default="default" type="defaultable-boolean">
        <xsd:attribute name="default-autowire" default="default">
			<xsd:simpleType>
				<xsd:restriction base="xsd:NMTOKEN">
					<xsd:enumeration value="default"/>
					<xsd:enumeration value="no"/>
					<xsd:enumeration value="byName"/>
					<xsd:enumeration value="byType"/>
					<xsd:enumeration value="constructor"/>
				</xsd:restriction>
			</xsd:simpleType>
		</xsd:attribute>


	<xsd:simpleType name="defaultable-boolean">
		<xsd:restriction base="xsd:NMTOKEN">
			<xsd:enumeration value="default"/>
			<xsd:enumeration value="true"/>
			<xsd:enumeration value="false"/>
		</xsd:restriction>
	</xsd:simpleType>

</xsd:schema>
```



### <a id="reference">References</a>
1. <a href="https://guides.github.com/features/mastering-markdown/" target="_blank">Mastering Markdown</a>