/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package SnowballThrower.dmxsoftware.Database;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import java.io.File;
import java.util.LinkedList;
import java.util.List;

/**
 *
 * @author Sven
 */
public class XMLReader {

    public List<Device> getDevices() {
        List list = new LinkedList();
        try {

            File fXmlFile = new File("Data/Devices.xml");
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            Document doc = dBuilder.parse(fXmlFile);

            //optional, but recommended
            doc.getDocumentElement().normalize();

            System.out.println("Root element :" + doc.getDocumentElement().getNodeName());

            NodeList devList = doc.getElementsByTagName("Device");

            System.out.println("----------------------------");

            for (int temp = 0; temp < devList.getLength(); temp++) {

                Node devNode = devList.item(temp);

                System.out.println("\nCurrent Element :" + devNode.getNodeName());

                if (devNode.getNodeType() == Node.ELEMENT_NODE) {
                    Device device = new Device();
                    Element devElement = (Element) devNode;

                    device.id = devElement.getAttribute("Id");
                    device.startChannel = Integer.parseInt((devElement.getAttribute("Channel")).replaceAll("\"", ""));
                    device.name = devElement.getElementsByTagName("Name").item(0).getTextContent();
                    device.addition = devElement.getElementsByTagName("Additional").item(0).getTextContent();
                    device.typeId = ((Element) devElement.getElementsByTagName("Type").item(0)).getAttribute("Id");
                    System.out.println(device.name + " " + device.typeId);
                    list.add(device);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return list;
    }

    public List<DeviceType> getTypes() {
        List<DeviceType> devices = new LinkedList();
        try {

            File fXmlFile = new File("Data/DeviceTypes.xml");
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            Document doc = dBuilder.parse(fXmlFile);

            //optional
            doc.getDocumentElement().normalize();

            System.out.println("Root element :" + doc.getDocumentElement().getNodeName());

            NodeList deviceNodeList = doc.getElementsByTagName("Device");

            System.out.println("----------------------------");

            for (int countDev = 0; countDev < deviceNodeList.getLength(); countDev++) {

                Node deviceNode = deviceNodeList.item(countDev);
                //System.out.println("\nCurrent Element: " + deviceNode.getNodeName());

                if (deviceNode.getNodeType() == Node.ELEMENT_NODE) {

                    Element deviceElement = (Element) deviceNode;
                    DeviceType dev = new DeviceType();

                    dev.id = deviceElement.getAttribute("Id");
                    dev.name = deviceElement.getElementsByTagName("Name")
                            .item(0).getTextContent();
                    dev.type = Type.getType(deviceElement.getElementsByTagName("Type")
                            .item(0).getTextContent());
                    dev.picturePath = deviceElement.getElementsByTagName("Symbol")
                            .item(0).getTextContent();
                    dev.power = Integer.parseInt(deviceElement.getElementsByTagName("Power")
                            .item(0).getTextContent().replaceAll("\"", ""));
                    System.out.println(dev.name);
                    Node channelMasterNode = deviceElement.getElementsByTagName("Channels").item(0);
                    Element channelMasterElement = (Element) channelMasterNode;
                    dev.channelNumber = Integer.parseInt(channelMasterElement.getAttribute("No"));
                    NodeList channelNodeList = channelMasterElement.getElementsByTagName("Channel");
                    Channel[] channels = new Channel[dev.channelNumber];

                    for (int countCh = 0; countCh < channelNodeList.getLength(); countCh++) {

                        Channel channel = new Channel();
                        Node channelNode = channelNodeList.item(countCh);
                        //System.out.println("\nCurrent Element: " + channelNode.getNodeName());

                        if (channelNode.getNodeType() == Node.ELEMENT_NODE) {

                            Element channelElement = (Element) channelNode;
                            channel.number = Integer.parseInt(channelElement.getAttribute("Nr"));
                            channel.funcion = Function.toFunction(channelElement.getAttribute("F"));
                            channel.name = channelElement.getElementsByTagName("ChName")
                                    .item(0).getTextContent();
                            System.out.println(channel.name);
                            channel.meanings = getMeanings(channelElement);
                            System.out.println();
                        }

                        channels[channel.number - 1] = channel;
                    }
                    dev.channels = channels;
                    //System.out.println("\nCurrent Element: " + deviceNode.getNodeName());
                    devices.add(countDev, dev);

                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return devices;
    }

    List<Meaning> getMeanings(Element channelElement) {
        List<Meaning> meanings = new LinkedList<Meaning>();

        NodeList meaningList = channelElement.getElementsByTagName("Values");
        //System.out.println(meaningList.getLength());
        for (int countM = 0; countM < meaningList.getLength(); countM++) {
            Meaning m = new Meaning();
            Node meaningNode = meaningList.item(countM);
            Element meaningElement = (Element) meaningNode;
            if (meaningNode.getNodeType() == Node.ELEMENT_NODE) {
                m.conditions = new LinkedList<Condition>();
                m.values = new LinkedList<Value>();
                try {
                    m.name = meaningElement.getElementsByTagName("Name").item(0).getTextContent();
                } catch (NullPointerException npex) {
                    m.name = null;
                }

                m.conditions = getConditions(meaningElement);

                NodeList valueList = meaningElement.getElementsByTagName("Value");
                System.out.println(m.name);
                for (int valueCount = 0; valueCount < valueList.getLength(); valueCount++) {
                    Value value = new Value();
                    value.value = ((Element) valueList.item(valueCount)).getTextContent();
                    value.min = Integer.parseInt((((Element) valueList.item(valueCount))
                            .getAttribute("low")).replaceAll("\"", ""));
                    value.max = Integer.parseInt((((Element) valueList.item(valueCount))
                            .getAttribute("high")).replaceAll("\"", ""));
                    System.out.println(value.min + " - " + value.max + ": " + value.value);
                    m.values.add(value);
                }
                meanings.add(m);

            }
        }
        return meanings;
    }

    List<Condition> getConditions(Element meaningElement) {
        List<Condition> conditions = new LinkedList();
        if (meaningElement.getElementsByTagName("conditions").item(0) != null) {
            NodeList conditionList = ((Element) meaningElement.getElementsByTagName("conditions").
                    item(0)).getElementsByTagName("condition");
            for (int conditionCount = 0; conditionCount
                    < conditionList.getLength(); conditionCount++) {
                Condition condition = new Condition();
                condition.ranges = new LinkedList<Range>();

                Element conditionElement = (Element) conditionList.item(conditionCount);

                condition.Channel = Integer.parseInt((conditionElement.getAttribute("Channel")).replaceAll("\"", ""));
                System.out.println("Channel " + condition.Channel);

                if (conditionElement != null) {
                    NodeList conditionRangeList = conditionElement.getElementsByTagName("range");
                    for (int rangeCount = 0; rangeCount
                            < conditionRangeList.getLength(); rangeCount++) {
                        Range range = new Range();
                        range.min = Integer.parseInt((((Element) conditionRangeList.item(rangeCount))
                                .getAttribute("min")).replaceAll("\"", ""));
                        range.max = Integer.parseInt((((Element) conditionRangeList.item(rangeCount))
                                .getAttribute("max")).replaceAll("\"", ""));
                        condition.ranges.add(range);
                        System.out.println(range.min + " - " + range.max);
                    }
                }

            }
        }
        return conditions;
    }
}
