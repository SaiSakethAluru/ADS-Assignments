const studentGrade = artifacts.require("StudentGrade");

module.exports = function (deployer) {
  deployer.deploy(studentGrade);
};
