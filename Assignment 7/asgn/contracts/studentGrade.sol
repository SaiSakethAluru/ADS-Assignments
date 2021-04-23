// SPDX-License-Identifier: MIT
pragma solidity >=0.4.22 <0.8.0;

contract StudentGrade {
    uint[5] subjects;
    uint[5] credits;
    string[5] grade;

    function setsubjects(uint16[5] memory _subjects) public {
        // uint16[5] memory _subjects = [60003,60016,60010,60042,47006];
        uint  _i = 0;
        for(_i = 0;_i<5;_i++){
            subjects[_i] = _subjects[_i];
        }
    }    

    function setcredits(uint16[5] memory _credits) public {
        // uint16[5] memory _subjects = [60003,60016,60010,60042,47006];
        uint  _i = 0;
        for(_i = 0;_i<5;_i++){
            credits[_i] = _credits[_i];
        }
    }

    function setgrade(uint subjectId, string memory _grade) public {
        grade[subjectId] = _grade;
    }

    function calculateSGPA() public view returns(uint) {
        uint _i = 0;
        uint _totalCred = 0;
        uint _totalGradePoints = 0;
        uint _gradePoint = 0;
        for(_i=0;_i<5;_i++){    
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("E"))){
                _gradePoint=10;
            }
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("A"))){
                _gradePoint=9;
            }
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("B"))){
                _gradePoint=8;
            }
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("C"))){
                _gradePoint=7;
            }
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("D"))){
                _gradePoint=6;
            }
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("P"))){
                _gradePoint=5;
            }
            if(keccak256(abi.encodePacked(grade[_i]))==keccak256(abi.encodePacked("F"))){
                _gradePoint=0;
            }
            _totalCred += credits[_i];
            _totalGradePoints += credits[_i] * _gradePoint;

        }
        return _totalGradePoints / _totalCred;
    }
}