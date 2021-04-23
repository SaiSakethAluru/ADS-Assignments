pragma solidity >=0.4.22 <0.7.0;

contract StudentInfo {
    string name;
    uint roll;
    string hall;
    uint abcd;

    function setName(string memory _name) public {
        name = _name;
    }
    function setRoll(uint _roll) public {
        roll = _roll;
        abcd = 2*_roll;
    }
    
    function setHall(string memory _hall) public {
        hall = _hall;
    }
    
    function getName() public view returns (string memory) {
        return name;
    }
    function getRoll() public view returns(uint) {
        return roll;
    }
    function getHall() public view returns(string memory) {
        return hall;
    }
    
    function helloWorld() public pure returns(string memory) {
        return "Hello World";
    }  

    function getABCD() public view returns(uint) {
        return abcd;
    }  
}